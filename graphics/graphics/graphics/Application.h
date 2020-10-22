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

private:
	Application();
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

private:
	bool mCreated;
	GLFWwindow* mWindow;
};

#endif // !APPLICATION_H
