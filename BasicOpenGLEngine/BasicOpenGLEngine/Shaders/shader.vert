#version 330                                   
                                                                    
layout (location = 0) in vec3 pos;
                                                            
out vec4 v_color;                              
                                                            
uniform mat4 projection;                
                                                            
uniform mat4 model;                    
                                                            
void main() {                                       
                                                            
    gl_Position = projection * model * vec4(pos.x, pos.y, pos.z , 1.0);
    v_color = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
}