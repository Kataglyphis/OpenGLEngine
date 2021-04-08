#version 330                                   
                                                                    
in vec4 v_color;                                
in vec2 tex_coord;
in vec3 normal;
in vec3 frag_pos;
in vec4 directional_light_space_pos;

out vec4 color;                                  

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light {

    vec3 color;
    float ambient_intensity;
    float diffuse_intensity;

};

struct DirectionalLight {
    
    Light base;
    vec3 direction;

};

struct PointLight {
    
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;

};

struct SpotLight {
    
    PointLight base;

    vec3 direction;
    float edge;

};

struct Material {

    float specular_intensity;
    float shininess;

};

uniform sampler2D first_texture;
uniform sampler2D directional_shadow_map;

uniform int point_light_count;
uniform int spot_light_count;


uniform DirectionalLight directional_light; 
uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform SpotLight spot_lights[MAX_SPOT_LIGHTS];

uniform Material material;

uniform vec3 eye_position;

float calc_directional_shadow_factor(DirectionalLight d_light) {
    
    vec3 proj_coords = directional_light_space_pos.xyz / directional_light_space_pos.w;
    proj_coords = (proj_coords * 0.5f) + 0.5f;
    float closest_depth = texture(directional_shadow_map, proj_coords.xy).r;
    float current_depth = proj_coords.z;

    vec3 normalized_normal = normalize(normal);
    vec3 light_dir = normalize(d_light.direction);

    float bias = max(0.5f * (1.f - dot(normalized_normal, light_dir)), 0.005f);

    //PCF
    float shadow = 0.0f;
    vec2 texel_size = 1.0f / textureSize(directional_shadow_map, 0);

    for(int x = -2; x <= 2; x++) {
        for(int y = -2; y <= 2; y++) {
            float closest_depth_neighbor = texture(directional_shadow_map, proj_coords.xy + vec2(x,y) * texel_size).r;
            shadow += current_depth > closest_depth_neighbor + bias  ? 1.0f : 0.0f;
        }
    }
    shadow /= 25.f;

    //float shadow = current_depth > closest_depth + bias  ? 1.0f : 0.0f;

    if(proj_coords.z > 1.0) {
        shadow = 0.0f;
    }

    return shadow;

}

vec4 calc_light_by_direction(Light light, vec3 direction, float shadow_factor) {
    
    vec4 ambient_color = vec4(light.color, 1.0f) * light.ambient_intensity;
    float diffuse_factor = max(dot(normalize(normal), normalize(direction)), 0.0f);
    vec4 diffuse_color = vec4(light.color, 1.0f) * light.diffuse_intensity * diffuse_factor;

    vec4 specular_color = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    if(diffuse_factor > 0.0f) {

        vec3 frag_to_eye = normalize(eye_position - frag_pos);
        vec3 reflected = normalize(reflect(direction, normalize(normal)));

        float specular_factor = dot(frag_to_eye, reflected);
        if(specular_factor > 0.0f) {

            specular_factor = pow(specular_factor, material.shininess);
            specular_color = vec4(light.color * material.specular_intensity * specular_factor, 1.0f);

        }
    }

    return  (ambient_color + (1.f - shadow_factor) * (diffuse_color + specular_color));

}


vec4 calc_directional_light() {
    
    float shadow_factor = calc_directional_shadow_factor(directional_light);
    return calc_light_by_direction(directional_light.base, directional_light.direction, shadow_factor);

}


vec4 calc_point_light(PointLight p_light) {
        
    vec3 direction = frag_pos - p_light.position;
    float distance = length(direction);
    direction = normalize(direction);

    vec4 color = calc_light_by_direction(p_light.base, direction, 1.0f);
    float attentuation = p_light.exponent * pow(distance,2) 
                                    +  p_light.linear        * distance
                                    +  p_light.constant;

    return (color / attentuation);

}


vec4 calc_spot_light(SpotLight s_light) {

    vec3 ray_direction = normalize(frag_pos - s_light.base.position);
    float s1_factor = dot(ray_direction, s_light.direction);

    if(s1_factor > s_light.edge) {
    
        vec4 color = calc_point_light(s_light.base);
        return color * (1.0f - (1.0f - s1_factor) * (1.0f / (1.0f - s_light.edge))) ;
    
    } else {

        return vec4(0.f,0.f,0.f,0.f);

    }

}


vec4 calc_spot_lights() {

    vec4 total_color = vec4(0.0f,0.0f,0.0f,0.0f);
    for(int i = 0; i < spot_light_count; i++) {

        total_color += calc_spot_light(spot_lights[i]);

    }

    return total_color;

}




vec4 calc_point_lights() {

    vec4 total_color = vec4(0.0f,0.0f,0.0f,0.0f);

    for(int i = 0; i < point_light_count; i++) {

        total_color += calc_point_light(point_lights[i]);

    }

    return total_color;
}



void main() {                                       
    
    vec4 final_color = calc_directional_light();
    final_color += calc_point_lights();
    final_color += calc_spot_lights();

    color = texture(first_texture, tex_coord) * final_color;

} 