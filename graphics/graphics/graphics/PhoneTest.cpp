#include "PhoneTest.h"

#include "Application.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <sstream>

const static char* VERTEX_PATH_CUBE = "F:\\evo\\github\\tr-cg\\graphics\\graphics\\graphics\\res\\shader\\PhongDrawCube.vert";
const static char* FRAGMENT_PATH_CUBE = "F:\\evo\\github\\tr-cg\\graphics\\graphics\\graphics\\res\\shader\\PhongDrawCube.frag";
const static char* VERTEX_PATH_LAMP = "F:\\evo\\github\\tr-cg\\graphics\\graphics\\graphics\\res\\shader\\PhongDrawLamp.vert";
const static char* FRAGMENT_PATH_LAMP = "F:\\evo\\github\\tr-cg\\graphics\\graphics\\graphics\\res\\shader\\PhongDrawLamp.frag";

// 点光源位置
const static glm::vec3 PointLightPositions[] = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.5f, 0.0f),
	glm::vec3(0.0f, 0.0f, 2.0f),
	glm::vec3(0.0f, -2.5f, 0.0f),
	glm::vec3(0.0f, 0.0f, -3.0f),
	glm::vec3(-3.5f, 0.0f, 0.0f)
};

// 光源颜色
const static glm::vec3 LightColors[] = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.5f, 0.5f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.5f, 0.5f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.5f, 0.0f, 0.5f)
};

// 控制开关灯
static unsigned int LightBits = 0;

const static bool IsLightOn(int index)
{
	return (LightBits >> index) & 1;
}

PhoneTest::PhoneTest()
	: mCamera(glm::vec3(0.0f, 0.0f, 3.0f))
{
	// 翻转y坐标，由于y坐标正常从底部到顶部是依次增大的，如果不翻转，计算偏移量时要反过来
	mCamera.setFlipY(true);

	// 1个定向光
	mDirectLight = DirectLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f));

	// 6个点光源
	for (int i = 0; i < 6; i++) {
		mPointLights[i] = PointLight(PointLightPositions[i], 0.05f * LightColors[i], 0.8f * LightColors[i], LightColors[i], 1.0f, 0.09f, 0.032f);
	}

	// 1个聚光灯
	mSpotLight = SpotLight(mCamera.getPosition(), mCamera.getForward(), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f),
		1.0f, 0.09f, 0.032f, cos(glm::radians(12.5f)), cos(glm::radians(15.0f)));

	// 1.vao
	glGenVertexArrays(1, &mVAOCube);
	glBindVertexArray(mVAOCube);
	glGenVertexArrays(1, &mVAOLamp);
	glBindVertexArray(mVAOLamp);

	// 2.vbo
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	// 顶点数据																 
	float vertices[] = {
		// ---- 位置 ----       ---- 法线 ----   
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 3.vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// 4.unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 5.shader
	mShaderCube = new Shader(VERTEX_PATH_CUBE, FRAGMENT_PATH_CUBE, true);
	mShaderLamp = new Shader(VERTEX_PATH_LAMP, FRAGMENT_PATH_LAMP, true);
	glEnable(GL_DEPTH_TEST);
}

PhoneTest::~PhoneTest()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDeleteVertexArrays(1, &mVAOCube);
	glDeleteVertexArrays(1, &mVAOLamp);
	glDeleteBuffers(1, &mVBO);
	delete mShaderCube;
	mShaderCube = nullptr;
	delete mShaderLamp;
	mShaderLamp = nullptr;
}

void PhoneTest::render()
{
	// 1.clear buffer
	glClearColor(0.0f, 0.34f, 0.57f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 2.shader
	// 给立方体一个青色塑料(Cyan Plastic)的材质
	mShaderCube->useProgram();
	mShaderCube->setUniformVec3("viewPos", mCamera.getPosition());
	mShaderCube->setUniformFloat("material.shininess", 32.0f);
	mShaderCube->setUniformVec3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
	mShaderCube->setUniformVec3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);

	// 平行光
	mDirectLight.setOn(IsLightOn(0));
	mDirectLight.draw(*mShaderCube, "dirLight");

	// 点光源
	for (int i = 0; i < 6; i++) {
		std::stringstream nameStream;
		nameStream << "pointLights[" << i << "]";
		std::string name = nameStream.str();
		mPointLights[i].setOn(IsLightOn(i + 1));
		mPointLights[i].draw(*mShaderCube, name);
	}

	// 聚光灯
	mSpotLight.setOn(IsLightOn(7));
	mSpotLight.setPosition(mCamera.getPosition());
	mSpotLight.setDirection(mCamera.getForward());
	mSpotLight.draw(*mShaderCube, "spotLight");

	glm::mat4 view = mCamera.getViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(mCamera.getZoom()), 1.0f * Application::GetInstance().getWindowWidth() / Application::GetInstance().getWindowHeight(), 0.1f, 100.0f);
	mShaderCube->setUniformMat4("projection", projection);
	mShaderCube->setUniformMat4("view", view);

	// 绘制立方体
	glBindVertexArray(mVAOCube);
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
	mShaderCube->setUniformMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	mShaderLamp->useProgram();
	mShaderLamp->setUniformMat4("projection", projection);
	mShaderLamp->setUniformMat4("view", view);

	// 绘制灯
	glBindVertexArray(mVAOLamp);
	for (int i = 0; i < 6; i++)
	{
		if (IsLightOn(i + 1))
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, PointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			mShaderLamp->setUniformMat4("model", model);
			mShaderLamp->setUniformVec3("lightColor", LightColors[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
}

void PhoneTest::onMouseMoved(float xPos, float yPos, float xOffset, float yOffset)
{
	mCamera.rotate(glm::vec3(xOffset, yOffset, 0));
}

void PhoneTest::onMouseScrolled(float xOffset, float yOffset)
{
	mCamera.zoom(yOffset);
}

void PhoneTest::onMovementUp(float deltaTime)
{
	mCamera.onMovementUp(deltaTime);
}

void PhoneTest::onMovementDown(float deltaTime)
{
	mCamera.onMovementDown(deltaTime);
}

void PhoneTest::onMovementLeft(float deltaTime)
{
	mCamera.onMovementLeft(deltaTime);
}

void PhoneTest::onMovementRight(float deltaTime)
{
	mCamera.onMovementRight(deltaTime);
}

void PhoneTest::onDigitKeyPressed(int digit)
{
	switch (digit) {
	case 1: LightBits ^= 1; break;
	case 2: LightBits ^= (1 << 1); break;
	case 3: LightBits ^= (1 << 2); break;
	case 4: LightBits ^= (1 << 3); break;
	case 5: LightBits ^= (1 << 4); break;
	case 6: LightBits ^= (1 << 5); break;
	case 7: LightBits ^= (1 << 6); break;
	case 8: LightBits ^= (1 << 7); break;
	default: break;
	}
}
