#include "test.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>

static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	printf("framebufferSizeCallback width:%d height:%d\n", width, height);
	glViewport(0, 0, width, height);
}

int testWindow()
{
	printf("testWindow begin\n");

	// 1. init
	glfwInit(); // init glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // opengl version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core profile: without backward compatibility
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for mac os
	glfwWindowHint(GLFW_RESIZABLE, true); // window size can be changed

	int major, minor, rev;
	glfwGetVersion(&major, &minor, &rev);
	printf("testWindow glfw version %d.%d.%d\n", major, minor, rev); // glfw version 3.3.2

	int screenWidth = 400;
	int screenHeight = 300;
	auto window = glfwCreateWindow(screenWidth, screenHeight, "Window", nullptr, nullptr); // create window
	if (window == nullptr) {
		printf("testWindow glfwCreateWindow error\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // make context current

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // init glad
		printf("testWindow gladLoadGLLoader error\n");
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, screenWidth, screenHeight); // set viewport

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); // set window size changed callback

	// 2. render
	while (!glfwWindowShouldClose(window)) { // render loop
		int static count = 0;
		++count;
		if (count % 100 == 0) {
			printf("testWindow loop...%d\n", count);
		}

		//processInput(window);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); // clear color buffer with RGBA

		glfwSwapBuffers(window); // swap buffer
		glfwPollEvents(); // event poll
	}

	// 3. cleanup
	glfwTerminate();

	printf("testWindow end\n");

	return 0;
}