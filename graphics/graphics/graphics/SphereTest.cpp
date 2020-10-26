#include "SphereTest.h"

#include "Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <vector>

const static char* VERTEX_PATH = "F:\\evo\\github\\tr-cg\\graphics\\graphics\\graphics\\res\\shader\\Sphere.vert";
const static char* FRAGMENT_PATH = "F:\\evo\\github\\tr-cg\\graphics\\graphics\\graphics\\res\\shader\\Sphere.frag";

const static float PI = 3.14159265358979323846f;

// 将球横纵划分成50X50的网格
const static int Y_SEGMENTS = 50;
const static int X_SEGMENTS = 50;

SphereTest::SphereTest() {
	// 1.vao
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	// 2.vbo
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	// 生成球的顶点
	std::vector<float> sphereVertices;
	for (int y = 0; y <= Y_SEGMENTS; y++) {
		for (int x = 0; x <= X_SEGMENTS; x++) {
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::sin(ySegment * PI) * std::cos(xSegment * 2.0f * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(ySegment * PI) * std::sin(xSegment * 2.0f * PI);
			sphereVertices.push_back(xPos);
			sphereVertices.push_back(yPos);
			sphereVertices.push_back(zPos);
		}
	}
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

	// 3.ebo
	glGenBuffers(1, &mEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	// 生成球的索引
	std::vector<int> sphereIndices;
	for (int i = 0; i < Y_SEGMENTS; i++) {
		for (int j = 0; j < X_SEGMENTS; j++) {
			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);

			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
		}
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);

	// 4.vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// 5.unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// 6.shader
	mShader = new Shader(VERTEX_PATH, FRAGMENT_PATH, true);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

SphereTest::~SphereTest() {
    glDisableVertexAttribArray(0);
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
    delete mShader;
    mShader = nullptr;
}

void SphereTest::render() const {
	// 1.clear color buffer
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// 2.shader
	mShader->useProgram();
	// 开启面剔除（只需要展示一个面，否则会有重合）
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// 3.draw with vao
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
	// 点阵模式
	//glPointSize(5);
	//glDrawElements(GL_POINTS, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
