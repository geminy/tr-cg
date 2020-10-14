#include "Application.h"
#include "Window.h"
#include "Triangle.h"
#include "Quadrangle.h"

int main()
{
	Application::getInstance().create();
	//Application::getInstance().render(*new Window());
	//Application::getInstance().render(*new Triangle());
	Application::getInstance().render(*new Quadrangle());
	Application::getInstance().destroy();

	return 0;
}
