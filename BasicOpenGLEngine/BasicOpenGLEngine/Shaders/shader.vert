#version 330                                   
                                                                    
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coord0;
layout (location = 2) in vec3 norm;

out vec4 v_color;   
out vec2 tex_coord;                           
out vec3 normal;

uniform mat4 projection;                
uniform mat4 view;                                                            
uniform mat4 model;                    
                                                            
void main() {                                       
                                                            
    gl_Position = projection * view * model * vec4(pos, 1.0);
    v_color = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);

    tex_coord = tex_coord0;
    normal = mat3(transpose(inverse(model))) * norm;
}