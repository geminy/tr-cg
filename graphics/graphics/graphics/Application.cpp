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

void Application::create(int width, int height, const std::string& title) {
	if (mCreated) {
		return;
	}
	mCreated = true;

	// 1.glfw
	int result = glfwInit();
	printf("Application create glfwInit:%d\n", result);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, true);

	int major, minor, rev;
	glfwGetVersion(&major, &minor, &rev);
	printf("Application create glfwGetVersion:%d.%d.%d\n", major, minor, rev);

	mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	printf("Application create glfwCreateWindow:%p\n", mWindow);
	glfwMakeContextCurrent(mWindow);
	glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallback);

	// 2.glad
	result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	printf("Application create gladLoadGLLoader:%d\n", result);

	// 3.gl
	glViewport(0, 0, width, height);
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

	while (!glfwWindowShouldClose(mWindow)) {
		renderer.render();

		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
}

Application::Application() : mCreated(false), mWindow(nullptr) {}
