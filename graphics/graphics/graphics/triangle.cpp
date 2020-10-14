#include "Triangle.h"

#include "Shader.h"

#include <glad/glad.h>

Triangle::Triangle() {
    // vao
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    // vbo
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

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

    mShader = new Shader(vertexShaderSource, fragmentShaderSource);

    // 线框模式
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

Triangle::~Triangle() {
    glDisableVertexAttribArray(0);
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    delete mShader;
    mShader = nullptr;
}

void Triangle::render() const {
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mShader->useProgram(); // use shader program

    glBindVertexArray(mVAO); // bind vao
    glDrawArrays(GL_TRIANGLES, 0, 3); // draw trangle
    glBindVertexArray(0); // unbind
}
