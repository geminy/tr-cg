#include "WindowTest.h"

#include <glad/glad.h>

WindowTest::WindowTest() {}

WindowTest::~WindowTest() {}

void WindowTest::render() {
	// 1.clear color buffer with RGBA
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
