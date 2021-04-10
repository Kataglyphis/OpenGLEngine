#include "Skybox.h"

Skybox::Skybox()
{
}

Skybox::Skybox(std::vector<std::string> face_locations)
{
	sky_shader = new Shader();
	sky_shader->create_from_files("Shaders/skybox.vert","Shaders/skybox.frag");

	uniform_projection = sky_shader->get_projection_location();
	uniform_view = sky_shader->get_view_location();

	//texture setup
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

	int width, height, bit_depth;

	for (size_t i = 0; i < 6; i++) {

		unsigned char* texture_data = stbi_load(face_locations[i].c_str(), &width, &height, &bit_depth, 0);
		if (!texture_data) {
			printf("Failed to find: %s\n", face_locations[i].c_str());
			return;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);

		stbi_image_free(texture_data);
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Mesh Setup
	unsigned int skybox_indices[] = {
		//front
		0,1,2,
		2,1,3,
		//right
		2,3,5,
		5,3,7,
		//back
		5,7,4,
		4,7,6,
		//left
		4,6,0,
		0,6,1,
		//top
		4,0,5,
		5,0,2,
		//bottom
		1,6,3,
		3,6,7
	};

	float skybox_vertices[] = {
		-1.0f, 1.0f, -1.0f,			0.0f, 0.0f,			0.0f,0.0f,0.0f,
		-1.0f, -1.0f, -1.0f,		0.0f, 0.0f,			0.0f,0.0f,0.0f,
		1.0f, 1.0f, -1.0f,			0.0f, 0.0f,			0.0f,0.0f,0.0f,
		1.0f, -1.0f, -1.0f,			0.0f, 0.0f,			0.0f,0.0f,0.0f,

		-1.0f, 1.0f, 1.0f,			0.0f, 0.0f,			0.0f,0.0f,0.0f,
		1.0f, 1.0f, 1.0f,			0.0f, 0.0f,			0.0f,0.0f,0.0f,
		-1.0f, -1.0f, 1.0f,			0.0f, 0.0f,			0.0f,0.0f,0.0f,
		1.0f, -1.0f, 1.0f,			0.0f, 0.0f,			0.0f,0.0f,0.0f
	}
}

void Skybox::draw_skybox(glm::mat4 view_matrix, glm::mat4 projection_matrix)
{
}

Skybox::~Skybox()
{
}
