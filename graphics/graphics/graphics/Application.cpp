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

static void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
	//printf("cursorPosCallback window:%p xPos:%f yPos:%f\n", window, xPos, yPos);
}

static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	//printf("scrollCallback window:%p xOffset:%f yOffset:%f\n", window, xOffset, yOffset);
}

static void processInput(GLFWwindow* window) {
	static int frameCount = 0;
	double currentFrame = glfwGetTime();
	static double lastFrame = currentFrame;
	double deltaTime = currentFrame - lastFrame;
	static double totalTime = 0;
	totalTime += deltaTime;
	lastFrame = currentFrame;
	if (frameCount++ > 0) {
		//printf("Application processInput frame:%d time:%.1f fps:%.1f\n", frameCount, deltaTime * 1000.0, frameCount / totalTime);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		printf("Application processInput escape\n");
		glfwSetWindowShouldClose(window, true);
	} else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		printf("Application processInput w\n");
	} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		printf("Application processInput s\n");
	} else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		printf("Application processInput a\n");
	} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		printf("Application processInput d\n");
	}
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
	glfwWindowHint(GLFW_RESIZABLE, false);

	int major, minor, rev;
	glfwGetVersion(&major, &minor, &rev);
	printf("Application create glfwGetVersion:%d.%d.%d\n", major, minor, rev);

	mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	printf("Application create glfwCreateWindow:%p\n", mWindow);
	glfwMakeContextCurrent(mWindow);

	glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallback);
	glfwSetCursorPosCallback(mWindow, cursorPosCallback);
	glfwSetScrollCallback(mWindow, scrollCallback);
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
		// 1.input
		processInput(mWindow);

		// 2.render
		renderer.render();

		// 3.swap buffer
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
}

Application::Application() : mCreated(false), mWindow(nullptr) {}
