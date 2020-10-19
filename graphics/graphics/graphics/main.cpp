#include "Application.h"
#include "WindowTest.h"
#include "TriangleTest.h"
#include "QuadrangleTest.h"
#include "SphereTest.h"
#include "ModelTest.h"

int main()
{
	Application::getInstance().create();
	//Application::getInstance().render(*new WindowTest());
	//Application::getInstance().render(*new TriangleTest());
	//Application::getInstance().render(*new QuadrangleTest());
	//Application::getInstance().render(*new SphereTest());
	Application::getInstance().render(*new ModelTest());
	//Application::getInstance().destroy();

	return 0;
}
