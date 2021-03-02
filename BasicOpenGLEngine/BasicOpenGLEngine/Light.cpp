#include "Light.h"

Light::Light() {

	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambient_intensity = 1.0f;

}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat a_intensity) {

	color = glm::vec3(red, green, blue);
	ambient_intensity = a_intensity;

}

void Light::use_light(GLfloat ambient_intensity_location, GLfloat ambient_color_location) {

	glUniform3f(ambient_color_location, color.x, color.y, color.z);
	glUniform1f(ambient_intensity_location, ambient_intensity);
}

Light::~Light() {

}