#include "SphereTest.h"

#include "Shader.h"

#include <glad/glad.h>

#include <cmath>
#include <vector>

const static char* VERTEX_PATH = "F:\\evo\\github\\tr-cg\\graphics\\graphics\\graphics\\res\\shader\\Sphere.vert";
const static char* FRAGMENT_PATH = "F:\\evo\\github\\tr-cg\\graphics\\graphics\\graphics\\res\\shader\\Sphere.frag";

const static float PI = 3.14159265358979323846f;

// 将球横纵划分成50X50的网格
const static int X_SEGMENTS = 50;
const static int Y_SEGMENTS = 50;

SphereTest::SphereTest() {
	// 1.vao
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	// 2.vbo
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	// 球的顶点
	std::vector<float> vertices;
	for (int y = 0; y <= Y_SEGMENTS; y++) {
		for (int x = 0; x <= X_SEGMENTS; x++) {
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::sin(ySegment * PI) * std::cos(xSegment * 2.0f * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(ySegment * PI) * std::sin(xSegment * 2.0f * PI);
			vertices.push_back(xPos);
			vertices.push_back(yPos);
			vertices.push_back(zPos);
		}
	}
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	// 3.ebo
	glGenBuffers(1, &mEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	// 球的索引
	std::vector<int> indices;
	for (int i = 0; i < Y_SEGMENTS; i++) {
		for (int j = 0; j < X_SEGMENTS; j++) {
			indices.push_back(i * (X_SEGMENTS + 1) + j);
			indices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
			indices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);

			indices.push_back(i * (X_SEGMENTS + 1) + j);
			indices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
			indices.push_back(i * (X_SEGMENTS + 1) + j + 1);
		}
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

	// 4.vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// 5.unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// 6.shader
	mShader = new Shader(VERTEX_PATH, FRAGMENT_PATH, true);
	mShader->enableLineMode();
	mShader->enableCullFace();
}

SphereTest::~SphereTest() {
    glDisableVertexAttribArray(0);
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
    delete mShader;
    mShader = nullptr;
}

void SphereTest::render()
{
	// 1.clear color buffer
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// 2.shader
	mShader->useProgram();

	// 3.draw with ebo
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
	// 点阵模式
	//glPointSize(5);
	//glDrawElements(GL_POINTS, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
