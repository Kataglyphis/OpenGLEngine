#include "Model.h"

void Model::load_model(const std::string& file_name)
{

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file_name, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals
																				| aiProcess_JoinIdenticalVertices);
	if (!scene) {
		printf("Model (%s) failed to load: %s", file_name, (char*) importer.GetErrorString());
		return;
	}

	load_node(scene->mRootNode, scene);

	load_materials(scene);
}

void Model::render_model()
{
}

void Model::clear_model()
{
}

Model::~Model()
{
}

void Model::load_node(aiNode* node, const aiScene* scene)
{

	for (size_t i = 0; i < node->mNumMeshes; i++) {
		load_mesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++) {
		load_node(node->mChildren[i], scene);
	}

}

void Model::load_mesh(aiMesh* mesh, const aiScene* scene)
{

	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		vertices.insert(vertices.end(), {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z});
		if (mesh->mTextureCoords[0]) {
			
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y});

		}
		else {

			vertices.insert(vertices.end(), { 0.0f, 0.0f });

		}
			vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* new_mesh = new Mesh();
	new_mesh->create_mesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	mesh_list.push_back(new_mesh);
	mesh_to_tex.push_back(mesh->mMaterialIndex);
}

void Model::load_materials(const aiScene* scene)
{

	texture_list.resize(scene->mNumMaterials);

}
