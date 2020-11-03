#include "Application.h"

#include "AbstractRenderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>

static AbstractRenderer* CurrentRenderer = nullptr;

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	printf("Application FramebufferSizeCallback window:%p width:%d height:%d\n", window, width, height);
	glViewport(0, 0, width, height);
	if (CurrentRenderer != nullptr) {
		CurrentRenderer->onWindowSizeChanged(width, height);
	}
}

static void CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	static float lastX = (float)xPos;
	static float lastY = (float)yPos;
	float xOffset = (float)xPos - lastX;
	float yOffset = (float)yPos - lastY;
	lastX = (float)xPos;
	lastY = (float)yPos;
	printf("Application CursorPosCallback window:%p xPos:%f yPos:%f xOffset:%f yOffset:%f\n", window, xPos, yPos, xOffset, yOffset);
	if (CurrentRenderer != nullptr) {
		CurrentRenderer->onMouseMoved((float)xPos, (float)yPos, xOffset, yOffset);
	}
}

static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	printf("Application ScrollCallback window:%p xOffset:%f yOffset:%f\n", window, xOffset, yOffset);
	if (CurrentRenderer != nullptr) {
		CurrentRenderer->onMouseScrolled((float)xOffset, (float)yOffset);
	}
}

static void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	printf("Application KeyCallback window:%p key:%d scanCode:%d action:%d mods:%d\n", window, key, scanCode, action, mods);
	if (CurrentRenderer != nullptr) {
		if (action == GLFW_PRESS) {
			switch (key) {
				case GLFW_KEY_1:
					CurrentRenderer->onDigitKeyPressed(1);
					break;
				case GLFW_KEY_2: 
					CurrentRenderer->onDigitKeyPressed(2);
					break;
				case GLFW_KEY_3: 
					CurrentRenderer->onDigitKeyPressed(3);
					break;
				case GLFW_KEY_4: 
					CurrentRenderer->onDigitKeyPressed(4);
					break;
				case GLFW_KEY_5:
					CurrentRenderer->onDigitKeyPressed(5);
					break;
				case GLFW_KEY_6:
					CurrentRenderer->onDigitKeyPressed(6);
					break;
				case GLFW_KEY_7: 
					CurrentRenderer->onDigitKeyPressed(7);
					break;
				case GLFW_KEY_8:
					CurrentRenderer->onDigitKeyPressed(8); 
					break;
				default:
					break;
			}
		}
	}
}

static void ProcessInput(GLFWwindow* window)
{
	static int frameCount = 0;
	double currentFrame = glfwGetTime();
	static double lastFrame = currentFrame;
	double deltaTime = currentFrame - lastFrame;
	static double totalTime = 0;
	totalTime += deltaTime;
	lastFrame = currentFrame;
	if (frameCount++ > 0) {
		//printf("Application ProcessInput frame:%d time:%.1f fps:%.1f\n", frameCount, deltaTime * 1000.0, frameCount / totalTime);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		printf("Application ProcessInput Esc\n");
		glfwSetWindowShouldClose(window, true);
	} else {
		if (CurrentRenderer != nullptr) {
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				printf("Application procProcessInputessInput W\n");
				CurrentRenderer->onMovementUp((float)deltaTime);
			}
			else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				printf("Application ProcessInput S\n");
				CurrentRenderer->onMovementDown((float)deltaTime);
			}
			else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				printf("Application ProcessInput A\n");
				CurrentRenderer->onMovementLeft((float)deltaTime);
			}
			else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				printf("Application ProcessInput D\n");
				CurrentRenderer->onMovementRight((float)deltaTime);
			}
		}
	}
}

Application& Application::GetInstance()
{
	static Application instance;
	return instance;
}

Application::~Application()
{
	mWindow = nullptr;
	CurrentRenderer = nullptr;
}

void Application::create(int width, int height, const std::string& title)
{
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

	mWidth = width;
	mHeight = height;
	mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	printf("Application create glfwCreateWindow:%p\n", mWindow);
	glfwMakeContextCurrent(mWindow);

	glfwSetFramebufferSizeCallback(mWindow, FramebufferSizeCallback);
	glfwSetCursorPosCallback(mWindow, CursorPosCallback);
	glfwSetScrollCallback(mWindow, ScrollCallback);
	glfwSetKeyCallback(mWindow, KeyCallback);
	//glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Òþ²Ø¹â±ê

	// 2.glad
	result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	printf("Application create gladLoadGLLoader:%d\n", result);
}

void Application::destroy()
{
	if (!mCreated) {
		return;
	}
	mCreated = false;

	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void Application::render(const AbstractRenderer& renderer)
{
	if (!mCreated) {
		printf("Application render error: not created\n");
		return;
	}

	CurrentRenderer = &const_cast<AbstractRenderer&>(renderer);
	
	printf("Application render begin\n");
	while (!glfwWindowShouldClose(mWindow)) {
		// 1.process input
		ProcessInput(mWindow);

		// 2.render
		CurrentRenderer->render();

		// 3.swap buffer
		glfwSwapBuffers(mWindow);

		// 4.poll event
		glfwPollEvents();
	}
	printf("Application render end\n");
}

int Application::getWindowWidth() const
{
	return mWidth;
}

int Application::getWindowHeight() const
{
	return mHeight;
}

Application::Application()
	: mWidth(0), mHeight(0), mCreated(false), mWindow(nullptr) {}
