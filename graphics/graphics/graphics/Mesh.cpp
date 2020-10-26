#include "Mesh.h"

#include "Shader.h"

#include <glad/glad.h>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
	: mVertices(vertices), mIndices(indices), mTextures(textures)
{
	// 1.vao
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	// 2.vbo
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

	// 3.ebo
	glGenBuffers(1, &mEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

	// 4.vertex attibute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	// 5.unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// TODO
Mesh::~Mesh() {
	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(2);
	//glDisableVertexAttribArray(3);
	//glDisableVertexAttribArray(4);
	//glDeleteVertexArrays(1, &mVAO);
	//glDeleteBuffers(1, &mVBO);
	//glDeleteBuffers(1, &mEBO);
}

void Mesh::draw(const Shader& shader) {
	unsigned int diffuse = 1;
	unsigned int specular = 1;
	unsigned int normal = 1;
	unsigned int height = 1;

	// 1.texture
	for (int i = 0; i < mTextures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string type = mTextures[i].getType();
		if (type == "texture_diffuse") {
			number = std::to_string(diffuse++);
		} else if (type == "texture_specular") {
			number = std::to_string(specular++);
		} else if (type == "texture_normal") {
			number = std::to_string(normal++);
		} else if (type == "texture_height") {
			number = std::to_string(height++);
		}
		shader.setUniformInt(type + number, i);
		glBindTexture(GL_TEXTURE_2D, mTextures[i].getId());
	}

	// 2.draw with vao
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)mIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}
