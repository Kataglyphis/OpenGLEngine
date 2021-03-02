#pragma once

#include <GL\glew.h>

#include "stb_image.h"

#include <string.h>

class Texture
{
public:
	Texture();
	Texture(char* file_loc);

	void load_texture();
	void use_texture();
	void clear_texture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bit_depth;

	char* file_location;
};

