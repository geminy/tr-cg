#include "Application.h"

#include "AbstractRenderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>

static AbstractRenderer* CurrentRenderer = nullptr;

static void CursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
	printf("CursorPosCallback window:%p xPos:%f yPos:%f\n", window, xPos, yPos);
	if (CurrentRenderer != nullptr) {
		CurrentRenderer->onMouseMoved((float)xPos, (float)yPos);
	}
}

static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	printf("ScrollCallback window:%p xOffset:%f yOffset:%f\n", window, xOffset, yOffset);
	if (CurrentRenderer != nullptr) {
		CurrentRenderer->onMouseScrolled((float)xOffset, (float)yOffset);
	}
}

static void ProcessInput(GLFWwindow* window) {
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
		if (CurrentRenderer != nullptr) {
			CurrentRenderer->onMovementUp((float)deltaTime);
		}
	} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		printf("Application processInput s\n");
		if (CurrentRenderer != nullptr) {
			CurrentRenderer->onMovementDown((float)deltaTime);
		}
	} else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		printf("Application processInput a\n");
		if (CurrentRenderer != nullptr) {
			CurrentRenderer->onMovementLeft((float)deltaTime);
		}
	} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		printf("Application processInput d\n");
		if (CurrentRenderer != nullptr) {
			CurrentRenderer->onMovementRight((float)deltaTime);
		}
	}
}

Application& Application::GetInstance() {
	static Application instance;
	return instance;
}

Application::~Application() {
	mWindow = nullptr;
	CurrentRenderer = nullptr;
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

	glfwSetCursorPosCallback(mWindow, CursorPosCallback);
	glfwSetScrollCallback(mWindow, ScrollCallback);
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

	CurrentRenderer = &const_cast<AbstractRenderer&>(renderer);
	
	while (!glfwWindowShouldClose(mWindow)) {
		// 1.input
		ProcessInput(mWindow);

		// 2.render
		renderer.render();

		// 3.swap buffer
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
}

Application::Application() : mCreated(false), mWindow(nullptr) {}
