#include "Sphere.h"

#include "Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <vector>

constexpr float PI = 3.14159265358979323846f;

// 将球横纵划分成50X50的网格
constexpr int Y_SEGMENTS = 50;
constexpr int X_SEGMENTS = 50;

Sphere::Sphere() {
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = vec4(aPos, 1.0);\n"
		"}\n\0";

	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"    FragColor = vec4(1.0f, 0.0f, 0.f, 1.0f);\n"
		"}\n\0";

	mShader = new Shader(vertexShaderSource, fragmentShaderSource);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

Sphere::~Sphere() {
    glDisableVertexAttribArray(0);
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
    delete mShader;
    mShader = nullptr;
}

void Sphere::render() const {
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	mShader->useProgram();

	// 开启面剔除（只需要展示一个面，否则会有重合）
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
	//点阵模式绘制
	//glPointSize(5);
	//glDrawElements(GL_POINTS, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
