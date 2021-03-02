#version 330                                   
                                                                    
in vec4 v_color;                                
in vec2 tex_coord;

out vec4 color;                                  

struct DirectionalLight {
    vec3 color;
    GLfloat ambient_intensity;
};

uniform sampler2D first_texture;
uniform DirectionalLight directional_light; 

void main() {                                       

    vec4 ambient_color = vec4(directional_light.color, 1.0f) * directional_light.ambient_intensity;                                                        
    color = texture(first_texture, tex_coord) * ambient_color;

} 