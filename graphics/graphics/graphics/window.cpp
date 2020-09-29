#include "test.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>

static int ScreenWdith = 400;
static int ScreenHeight = 300;

static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	printf("framebufferSizeCallback width:%d height:%d\n", width, height);
	glViewport(0, 0, width, height);
}

int testWindow()
{
	printf("testWindow begin\n");

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, true);

	int major, minor, rev;
	glfwGetVersion(&major, &minor, &rev);
	printf("testWindow glfw version %d.%d.%d\n", major, minor, rev);

	auto window = glfwCreateWindow(ScreenWdith, ScreenHeight, "Window", nullptr, nullptr);
	if (window == nullptr) {
		printf("testWindow glfwCreateWindow error\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("testWindow gladLoadGLLoader error\n");
		return -1;
	}

	glViewport(0, 0, ScreenWdith, ScreenHeight);

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	while (!glfwWindowShouldClose(window)) {
		int static count = 0;
		++count;
		if (count % 100 == 0) {
			printf("testWindow loop...%d\n", count);
		}

		//processInput(window);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	printf("testWindow end\n");

	return 0;
}