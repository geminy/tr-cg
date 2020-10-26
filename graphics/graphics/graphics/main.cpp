#include "Application.h"
#include "WindowTest.h"
#include "TriangleTest.h"
#include "QuadrangleTest.h"
#include "SphereTest.h"
#include "ModelTest.h"
#include "CubeTest.h"
#include "CameraTest.h"

int main()
{
	Application::GetInstance().create();
	//Application::getInstance().render(*new WindowTest());
	//Application::getInstance().render(*new TriangleTest());
	//Application::getInstance().render(*new QuadrangleTest());
	//Application::getInstance().render(*new SphereTest());
	//Application::GetInstance().render(*new ModelTest());
	//Application::GetInstance().render(*new CubeTest());
	Application::GetInstance().render(*new CameraTest());
	//Application::getInstance().destroy();

	return 0;
}
