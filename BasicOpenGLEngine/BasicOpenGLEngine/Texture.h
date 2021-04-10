#pragma once

#include <GL\glew.h>

#include <string.h>

#include "GlobalValues.h"

class Texture
{
public:
	Texture();
	Texture(const char* file_loc);

	bool load_texture();
	bool load_texture_A();

	void use_texture();
	void clear_texture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bit_depth;

	const char* file_location;
};

