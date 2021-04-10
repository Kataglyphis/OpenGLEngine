#include "Light.h"

Light::Light() {

	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambient_intensity = 1.0f;
	diffuse_intensity = 0.0f;

}

Light::Light(GLfloat shadow_width, GLfloat shadow_height, 
					  GLfloat red, GLfloat green, GLfloat blue,	
					  GLfloat a_intensity, GLfloat d_intensity) {
	shadow_map = new ShadowMap();
	shadow_map->init((GLuint)shadow_width, (GLuint)shadow_height);

	color = glm::vec3(red, green, blue);
	ambient_intensity = a_intensity;
	diffuse_intensity = d_intensity;

}

Light::~Light() {

}