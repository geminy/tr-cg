#include "Window.h"

#include <glad/glad.h>

void Window::render() const {
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // clear color buffer with RGBA
}
