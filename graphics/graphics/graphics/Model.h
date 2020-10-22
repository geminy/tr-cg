#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "Texture.h"

#include <assimp/scene.h>

#include <string>
#include <vector>

class Shader;

class Model
{
public:
	Model(const std::string& path);
	~Model();

	void draw(const Shader& shader);

private:
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);

private:
	std::string mDirectory;
	std::vector<Mesh> mMeshes;
	std::vector<Texture> mTexturesLoaded;
};
#endif // !MODEL_H
