#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GlobalValues.h"

#include "Mesh.h"
#include "Shader.h"

class Skybox
{
public:
	Skybox();

	Skybox(std::vector<std::string> face_locations);

	void draw_skybox(glm::mat4 view_matrix, glm::mat4 projection_matrix);

	~Skybox();

private:
	Mesh* sky_mesh;
	Shader* sky_shader;

	GLuint texture_id;
	GLuint uniform_projection, uniform_view;
};

