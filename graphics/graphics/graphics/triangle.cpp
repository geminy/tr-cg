#include "test.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>

int testTriangle() {
    printf("testTriangle begin\n");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    int screenWidth = 400;
    int screenHeight = 300;
    auto window = glfwCreateWindow(screenWidth, screenHeight, "Triangle", nullptr, nullptr);
    if (window == nullptr) {
        printf("testTriangle glfwCreateWindow error\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("testTriangle gladLoadGLLoader error\n");
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, screenWidth, screenHeight);

    // vao
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // vbo
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

    // bind vertex data to current buffer
    float triangle[] = {
        -0.5f, -0.5f, 0.0f, // 左下
        0.5f, -0.5f, 0.0f, // 右下
        0.0f,  0.5f, 0.0f // 正上
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    // set vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // shader
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
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    int vSuccess;
    char vInfoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vSuccess);
    if (!vSuccess) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, vInfoLog);
        printf("testTriangle compile vertex shader error:%s\n", vInfoLog);
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    int fSuccess;
    char fInfoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fSuccess);
    if (!fSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, fInfoLog);
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
        glGetProgramInfoLog(shaderProgram, 512, nullptr, pInfoLog);
        printf("testTriangle link program error:%s\n", pInfoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 线框模式
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram); // use shader program

        glBindVertexArray(vertexArrayObject); // bind vao
        glDrawArrays(GL_TRIANGLES, 0, 3); // draw trangle
        glBindVertexArray(0); // unbind

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // cleanup
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);

    glfwTerminate();

    printf("testTriangle end\n");

    return 0;
}