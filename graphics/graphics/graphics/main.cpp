#include "Application.h"
#include "WindowTest.h"
#include "TriangleTest.h"
#include "QuadrangleTest.h"
#include "SphereTest.h"
#include "ModelTest.h"
#include "CubeTest.h"
#include "CameraTest.h"
#include "PhoneTest.h"

int main()
{
	Application::GetInstance().create();
	//Application::GetInstance().render(*new WindowTest()); // 1
	Application::GetInstance().render(*new TriangleTest()); // 2
	//Application::GetInstance().render(*new QuadrangleTest()); // 3
	//Application::GetInstance().render(*new SphereTest()); // 4
	//Application::GetInstance().render(*new ModelTest()); // 5
	//Application::GetInstance().render(*new CubeTest()); // 6
	//Application::GetInstance().render(*new CameraTest()); // 7
	//Application::GetInstance().render(*new PhoneTest()); // 8
	Application::GetInstance().destroy();

	return 0;
}
