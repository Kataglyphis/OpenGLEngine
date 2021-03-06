#pragma once

#include <GL\glew.h>

#include "stb_image.h"

#include <string.h>

class Texture
{
public:
	Texture();
	Texture(char* file_loc);

	bool load_texture();
	bool load_texture_A();

	void use_texture();
	void clear_texture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bit_depth;

	char* file_location;
};

