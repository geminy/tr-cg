#include "test.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>

static int ScreenWidth = 400;
static int ScreenHeight = 300;

int testTriangle() {
    printf("testTriangle begin\n");

    // 1. initialize
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    auto window = glfwCreateWindow(ScreenWidth, ScreenHeight, "Triangle", nullptr, nullptr);
    if (window == nullptr) {
        printf("testTriangle glfwCreateWindow error\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("testTriangle gladLoadGLLoader error\n");
        return -1;
    }

    glViewport(0, 0, ScreenWidth, ScreenHeight);

    // 2. VAO/VBO
    const float triangle[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 3. shader
    const char* vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos, 1.0);\n"
        "}\n\0";
    const char* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.0f, 0.f, 1.0f);\n"
        "}\n\0";

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int vSuccess;
    char vInfoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vSuccess);
    if (!vSuccess) {
        glGetShaderInfoLog(vertexShader, 512, NULL, vInfoLog);
        printf("testTriangle compile vertex shader error:%s\n", vInfoLog);
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    int fSuccess;
    char fInfoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fSuccess);
    if (!fSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, fInfoLog);
        printf("testTriangle compile fragment shader error:%s\n", fInfoLog);
    }

    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    int pSuccess;
    char pInfoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &pSuccess);
    if (!pSuccess) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, pInfoLog);
        printf("testTriangle link program error:%s\n", pInfoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Ïß¿òÄ£Ê½
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 4. render
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(vertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // 5. cleanup
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);

    glfwTerminate();

    return 0;
}