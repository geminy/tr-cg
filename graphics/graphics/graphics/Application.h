#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>

class AbstractRenderer;
struct GLFWwindow;

class Application
{
public:
	static Application& GetInstance();

	~Application();

	void create(int width = 1280, int height = 720, const std::string& title = "Graphics");
	void destroy();

	void render(const AbstractRenderer& renderer);

	int getWindowWidth() const;
	int getWindowHeight() const;

private:
	Application();
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

private:
	int mWidth;
	int mHeight;
	bool mCreated;
	GLFWwindow* mWindow;
};

#endif // !APPLICATION_H
