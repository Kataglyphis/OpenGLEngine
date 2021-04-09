#version 330

in vec4 FragPos;

uniform vec3 light_pos;
uniform float far_plane;

void main() {

	float distance = length(FragPos.xyz - light_pos); 
	distance = distance/far_plane;
	gl_FragDepth = distance;
}