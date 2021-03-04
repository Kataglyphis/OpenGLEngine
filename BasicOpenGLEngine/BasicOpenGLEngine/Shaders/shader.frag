#version 330                                   
                                                                    
in vec4 v_color;                                
in vec2 tex_coord;
in vec3 normal;
in vec3 frag_pos;

out vec4 color;                                  

const int MAX_POINT_LIGHTS = 3;

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

struct Material {

    float specular_intensity;
    float shininess;

};

uniform sampler2D first_texture;

uniform int point_light_count;

uniform DirectionalLight directional_light; 
uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform Material material;

uniform vec3 eye_position;

vec4 calc_light_by_direction(Light light, vec3 direction) {
    
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

    return  (ambient_color + diffuse_color + specular_color);

}

vec4 calc_directional_light() {

    return calc_light_by_direction(directional_light.base, directional_light.direction);

}

vec4 calc_point_lights() {

    vec4 total_color = vec4(0.0f,0.0f,0.0f,0.0f);

    for(int i = 0; i < point_light_count; i++) {
        
        vec3 direction = frag_pos - point_lights[i].position;
        float distance = length(direction);
        direction = normalize(direction);

        vec4 color = calc_light_by_direction(point_lights[i].base, direction);
        float attentuation = point_lights[i].exponent * pow(distance,2) 
                                        +  point_lights[i].linear        * distance
                                        +  point_lights[i].constant;

        total_color += (color / attentuation);

    }

    return total_color;
}

void main() {                                       
    
    vec4 final_color = calc_directional_light();
    final_color += calc_point_lights();

    color = texture(first_texture, tex_coord) * final_color;

} 