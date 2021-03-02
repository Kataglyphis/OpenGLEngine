#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

class Light
{

public:

	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat a_intensity);

	void use_light(GLfloat ambient_intensity_location, GLfloat ambient_color_location);

	~Light();

private:

	glm::vec3 color;
	GLfloat ambient_intensity;

};

