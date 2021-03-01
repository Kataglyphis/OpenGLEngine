#pragma once
#include <GL\glew.h>

class Mesh
{
public:
	Mesh();

	void create_mesh(GLfloat* vertices, unsigned int* indices, unsigned int num_of_vertices, unsigned int num_of_indices);
	void render_mesh();
	void clear_mesh();

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei index_count;

};

