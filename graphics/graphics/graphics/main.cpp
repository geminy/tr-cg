#include "Application.h"
#include "WindowTest.h"
#include "TriangleTest.h"
#include "QuadrangleTest.h"
#include "SphereTest.h"
#include "ModelTest.h"

int main()
{
	Application::GetInstance().create();
	//Application::getInstance().render(*new WindowTest());
	//Application::getInstance().render(*new TriangleTest());
	//Application::getInstance().render(*new QuadrangleTest());
	//Application::getInstance().render(*new SphereTest());
	Application::GetInstance().render(*new ModelTest());
	//Application::getInstance().destroy();

	return 0;
}
