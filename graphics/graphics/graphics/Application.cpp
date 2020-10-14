#include "Application.h"

#include "AbstractRenderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>

static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	printf("framebufferSizeCallback window:%p width:%d height:%d\n", window, width, height);
	glViewport(0, 0, width, height);
}

Application& Application::getInstance() {
	static Application instance;
	return instance;
}

Application::~Application() {
	mWindow = nullptr;
}

void Application::create(int width, int height, const char* title) {
	if (mCreated) {
		return;
	}
	mCreated = true;

	int result = glfwInit(); // init glfw
	printf("Application create glfwInit:%d\n", result);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // opengl version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core profile: without backward compatibility
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for mac os
	glfwWindowHint(GLFW_RESIZABLE, true); // window size can be changed

	int major, minor, rev;
	glfwGetVersion(&major, &minor, &rev);
	printf("Application create glfwGetVersion:%d.%d.%d\n", major, minor, rev); // glfw version 3.3.2

	mWindow = glfwCreateWindow(width, height, title, nullptr, nullptr); // create window
	printf("Application create glfwCreateWindow:%p\n", mWindow);
	glfwMakeContextCurrent(mWindow); // make context current
	glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallback); // set window size changed callback

	result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // init glad
	printf("Application create gladLoadGLLoader:%d\n", result);

	glViewport(0, 0, width, height); // set viewport
}

void Application::destroy() {
	if (!mCreated) {
		return;
	}
	mCreated = false;

	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void Application::render(const AbstractRenderer& renderer) {
	if (!mCreated) {
		return;
	}

	while (!glfwWindowShouldClose(mWindow)) { // render loop
		renderer.render();

		glfwSwapBuffers(mWindow); // swap buffer
		glfwPollEvents(); // event poll
	}
}

Application::Application() : mCreated(false), mWindow(nullptr) {}
