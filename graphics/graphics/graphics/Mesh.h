#ifndef MESH_H
#define MESH_H

#include "Texture.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

class Shader;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
	~Mesh();

	void draw(const Shader& shader);

private:
	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int mEBO;
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	std::vector<Texture> mTextures;
};

#endif // !MESH_H
