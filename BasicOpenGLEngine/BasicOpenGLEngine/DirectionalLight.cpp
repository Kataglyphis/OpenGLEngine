#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{

	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	light_proj = glm::ortho(-20.f, 20.f, -20.f, 20.0f, 0.1f, 100.0f);
}


DirectionalLight::DirectionalLight(GLfloat shadow_width, GLfloat shadow_height, 
															GLfloat red, GLfloat green, GLfloat blue,
															GLfloat a_intensity, GLfloat d_intensity,
															GLfloat x_dir, GLfloat y_dir, GLfloat z_dir) : Light(shadow_width, shadow_height, 
																																				   red, green, blue,
																																				   a_intensity, d_intensity) {

	direction = glm::vec3(x_dir, y_dir, z_dir);
	light_proj = glm::ortho(-20.f, 20.f, -20.f, 20.0f, 0.1f, 100.0f);

}

void DirectionalLight::use_light(GLuint ambient_intensity_location, GLuint ambient_color_location,
	GLuint diffuse_intensity_location, GLuint direction_location) {

	glUniform3f(ambient_color_location, color.x, color.y, color.z);
	glUniform1f(ambient_intensity_location, ambient_intensity);
	glUniform3f(direction_location, direction.x, direction.y, direction.z);
	glUniform1f(diffuse_intensity_location, diffuse_intensity);

}

glm::mat4 DirectionalLight::calculate_light_transform()
{
	return light_proj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::~DirectionalLight()
{
}
