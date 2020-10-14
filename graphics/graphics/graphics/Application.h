#ifndef APPLICATION_H
#define APPLICATION_H

class AbstractRenderer;
struct GLFWwindow;

class Application
{
public:
	static Application& getInstance();

	~Application();

	void create(int width = 500, int height = 500, const char* title = "Graphics");
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
