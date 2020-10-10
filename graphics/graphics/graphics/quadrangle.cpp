#include "test.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>

int testQuadrangle() {
    printf("testQuadrangle begin\n");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    int screenWidth = 400;
    int screenHeight = 300;
    auto window = glfwCreateWindow(screenWidth, screenHeight, "Quadrangle", nullptr, nullptr);
    if (window == nullptr) {
        printf("testQuadrangle glfwCreateWindow error\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("testQuadrangle gladLoadGLLoader error\n");
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, screenWidth, screenHeight);

    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

    // 四边形的顶点数据
    float vertices[] = {
        // 第一个三角形
        0.5f, 0.5f, 0.0f,     // 右上
        0.5f, -0.5f, 0.0f,    // 右下
        -0.5f, -0.5f, 0.0f,   // 左下
        // 第二个三角形
        -0.5f, -0.5f, 0.0f,   // 左下（重复）
        0.5f, 0.5f, 0.0f,     // 右上（重复）
        -0.5f, 0.5f, 0.0f     // 左上
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ebo
    GLuint elementBufferObject;
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);

    // 索引数据
    unsigned int indices[] = {
        0, 1, 5,              // 第一个三角形
        1, 2, 5               // 第二个三角形
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
        printf("testQuadrangle compile vertex shader error:%s\n", vInfoLog);
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    int fSuccess;
    char fInfoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fSuccess);
    if (!fSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, fInfoLog);
        printf("testQuadrangle compile fragment shader error:%s\n", vInfoLog);
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
        printf("testQuadrangle link program error:%s\n", pInfoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(vertexArrayObject);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // using ebo
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &elementBufferObject);

    glfwTerminate();

    printf("testQuadrangle end\n");

    return 0;
}