#include "Texture.h"

Texture::Texture() {

	textureID = 0;
	width = 0;
	height = 0;
	bit_depth = 0;
	file_location = _strdup("");

}

Texture::Texture(const char* file_loc) {

	textureID = 0;
	width = 0;
	height = 0;
	bit_depth = 0;
	file_location = file_loc;

}

void Texture::use_texture() {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

}

bool Texture::load_texture() {

	unsigned char* texture_data = stbi_load(file_location, &width, &height, &bit_depth, 0);
	if (!texture_data) {
		printf("Failed to find: %s\n", file_location);
		return false;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texture_data);

	return true;
}

bool Texture::load_texture_A()
{
	unsigned char* texture_data = stbi_load(file_location, &width, &height, &bit_depth, 0);
	if (!texture_data) {
		printf("Failed to find: %s\n", file_location);
		return false;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texture_data);

	return true;
}

void Texture::clear_texture() {

	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bit_depth = 0;
	file_location = _strdup("");

}

Texture::~Texture() {
	clear_texture();
}