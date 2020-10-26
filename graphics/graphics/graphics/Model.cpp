#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <cstdio>

Model::Model(const std::string& path)
{
	// 1.file
	printf("Model path:%s\n", path.c_str());
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		printf("Model assimp error:%s %s\n", path.c_str(), importer.GetErrorString());
		return;
	}
	mDirectory = path.substr(0, path.find_last_of('\\'));

	// 2.node
	processNode(scene->mRootNode, scene);
}

// TODO
Model::~Model() {

}

void Model::draw(const Shader& shader)
{
	for (unsigned int i = 0; i < mMeshes.size(); i++) {
		mMeshes[i].draw(shader);
	}
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// 1.node
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		mMeshes.push_back(processMesh(mesh, scene));
	}

	// 2.children node
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// 1.vertex
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector3;

		// 1.1.vertex position
		vector3.x = mesh->mVertices[i].x;
		vector3.y = mesh->mVertices[i].y;
		vector3.z = mesh->mVertices[i].z;
		vertex.position = vector3;

		// 1.2.vertex normal
		vector3.x = mesh->mNormals[i].x;
		vector3.y = mesh->mNormals[i].y;
		vector3.z = mesh->mNormals[i].z;
		vertex.normal = vector3;

		// 1.3.vertex texCoords
		if (mesh->mTextureCoords[0]) {
			glm::vec2 vector2;

			vector2.x = mesh->mTextureCoords[0][i].x;
			vector2.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vector2;
		}
		else {
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}

		// 1.4.vertex tangent
		vector3.x = mesh->mTangents[i].x;
		vector3.y = mesh->mTangents[i].y;
		vector3.z = mesh->mTangents[i].z;
		vertex.tangent = vector3;

		// 1.5.vertex bitangent
		vector3.x = mesh->mBitangents[i].x;
		vector3.y = mesh->mBitangents[i].y;
		vector3.z = mesh->mBitangents[i].z;
		vertex.bitangent = vector3;

		vertices.push_back(vertex);
	}

	// 2.index
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// 3.texture
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		for (unsigned int j = 0; j < mTexturesLoaded.size(); j++) {
			if (std::strcmp(mTexturesLoaded[j].getName().data(), str.C_Str()) == 0) {
				textures.push_back(mTexturesLoaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip) {
			std::string path = mDirectory + '\\' + std::string(str.C_Str());
			Texture texture(path, typeName, std::string(str.C_Str()));
			textures.push_back(texture);
			mTexturesLoaded.push_back(texture);
		}
	}

	return textures;
}
