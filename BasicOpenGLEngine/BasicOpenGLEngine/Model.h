#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:

	Model();

	void load_model(const std::string& file_name);
	void render_model();
	void clear_model();

	~Model();

private:

	void load_node(aiNode* node, const aiScene* scene);
	void load_mesh(aiMesh* mesh, const aiScene* scene);
	void load_materials(const aiScene* scene);

	std::vector<Mesh*> mesh_list;
	std::vector<Texture*> texture_list;
	std::vector<unsigned int> mesh_to_tex;

};

