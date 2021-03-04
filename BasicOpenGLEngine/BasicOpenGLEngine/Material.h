#pragma once

#include <GL\glew.h>

class Material
{

public:

	Material();
	Material(GLfloat spec_intensity, GLfloat shininess_factor);

	void use_material(GLuint specular_intensity_location, GLuint shininess_location);

	~Material();

private:

	GLfloat specular_intensity;
	GLfloat shininess;

};

