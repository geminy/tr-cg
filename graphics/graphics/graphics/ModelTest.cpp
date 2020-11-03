#include "ModelTest.h"

#include "Application.h"
#include "Shader.h"
#include "Model.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const static char* VERTEX_PATH = "F:\\evo\\github\\tr-cg\\graphics\\graphics\\graphics\\res\\shader\\Model.vert";
const static char* FRAGMENT_PATH = "F:\\evo\\github\\tr-cg\\graphics\\graphics\\graphics\\res\\shader\\Model.frag";
const static char* MODEL_PATH = "F:\\evo\\github\\tr-cg\\graphics\\graphics\\graphics\\res\\model\\character.obj";

// TODO
const static glm::vec3 LightPos(8.0f, 8.0f, 5.0f);

ModelTest::ModelTest()
	: mCamera(glm::vec3(0.0f, 0.0f, 3.0f))
{
	// 1.shader
	mShader = new Shader(VERTEX_PATH, FRAGMENT_PATH, true);
	// ¿ªÆôÉî¶È²âÊÔ
	glEnable(GL_DEPTH_TEST);

	// 2.model
	mModel = new Model(MODEL_PATH);
}

ModelTest::~ModelTest() {
	delete mShader;
	mShader = nullptr;
	delete mModel;
	mModel = nullptr;
}

void ModelTest::render() {
	// 1.clear buffer
	glClearColor(0.0f, 0.34f, 0.57f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 2.shader
	mShader->useProgram();

	// 3.model view projection
	glm::mat4 model(1);
	model = glm::translate(model, glm::vec3(0.0f, -0.7f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	glm::mat4 view(1);
	view = mCamera.getViewMatrix();
	glm::mat4 projection(1);
	projection = glm::perspective(glm::radians(mCamera.getZoom()), 1.0f * Application::GetInstance().getWindowWidth() / Application::GetInstance().getWindowHeight(), 0.1f, 100.0f);
	mShader->setUniformMat4("model", model);
	mShader->setUniformMat4("view", view);
	mShader->setUniformMat4("projection", projection);
	mShader->setUniformVec3("lightPos", LightPos);
	mShader->setUniformVec3("viewPos", mCamera.getPosition());

	// 4.draw
	mModel->draw(*mShader);
}

void ModelTest::onMouseMoved(float xPos, float yPos, float xOffset, float yOffset)
{
	mCamera.onMouseMoved(xOffset, yOffset);
}

void ModelTest::onMouseScrolled(float xOffset, float yOffset)
{
	mCamera.onMouseScrolled(xOffset, yOffset);
}

void ModelTest::onMovementUp(float deltaTime)
{
	mCamera.onMovementUp(deltaTime);
}

void ModelTest::onMovementDown(float deltaTime)
{
	mCamera.onMovementDown(deltaTime);
}

void ModelTest::onMovementLeft(float deltaTime)
{
	mCamera.onMovementLeft(deltaTime);
}

void ModelTest::onMovementRight(float deltaTime)
{
	mCamera.onMovementRight(deltaTime);
}
