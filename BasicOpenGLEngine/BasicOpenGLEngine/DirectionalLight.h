#pragma once

#include "Light.h"

class DirectionalLight : public Light
{

public:

	DirectionalLight();

	DirectionalLight(GLfloat shadow_width, GLfloat shadow_height, 
								  GLfloat red, GLfloat green, GLfloat blue,
								  GLfloat a_intensity, GLfloat d_intensity,
								  GLfloat x_dir, GLfloat y_dir, GLfloat z_dir);

	void use_light(GLuint ambient_intensity_location, GLuint ambient_color_location,
		GLuint diffuse_intensity_location, GLuint direction_location);

	glm::mat4 calculate_light_transform();

	~DirectionalLight();

private:

	glm::vec3 direction;

};

