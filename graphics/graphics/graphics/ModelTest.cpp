#include "ModelTest.h"

#include <glad/glad.h>

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Texture.h"

const static char* VERTEX_PATH = "F:\\evo\\github\\tr-cg\\graphics\\graphics\\graphics\\res\\shader\\Model.vert";
const static char* FRAGMENT_PATH = "F:\\evo\\github\\tr-cg\\graphics\\graphics\\graphics\\res\\shader\\Model.frag";
const static char* MODEL_PATH = "F:\\evo\\github\\tr-cg\\graphics\\graphics\\graphics\\res\\model\\character.obj";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

float screen_width = 1280.0f;
float screen_height = 720.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;
bool firstMouse = true;

glm::vec3 lightPos(8.0f, 8.0f, 5.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

ModelTest::ModelTest() {
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

void ModelTest::render() const {
	glClearColor(0.0f, 0.34f, 0.57f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mShader->useProgram();

	glm::mat4 model(1);
	model = glm::translate(model, glm::vec3(0.0f, -0.7f, 0.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	glm::mat4 view(1);
	view = camera.GetViewMatrix();
	glm::mat4 projection(1);
	projection = glm::perspective(glm::radians(camera.Zoom), screen_width / screen_height, 0.1f, 100.0f);

	mShader->setUniformMat4("model", model);
	mShader->setUniformMat4("view", view);
	mShader->setUniformMat4("projection", projection);
	mShader->setUniformVec3("lightPos", lightPos);
	mShader->setUniformVec3("viewPos", camera.Position);

	mModel->Draw(*mShader);
}
