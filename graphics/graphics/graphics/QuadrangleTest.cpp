#include "QuadrangleTest.h"

#include "Shader.h"

#include <glad/glad.h>

QuadrangleTest::QuadrangleTest() {
    // 1.vao
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    // 2.vbo
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    // 四边形 = 两个三角形
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

    // 3.ebo
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    // 使用索引（可去除重复的顶点数据）
    unsigned int indices[] = {
        0, 1, 5,              // 第一个三角形
        1, 2, 5               // 第二个三角形
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 4.vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // 5.unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // 6.shader
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
    //mShader->enableLineMode(); // 线框模式
}

QuadrangleTest::~QuadrangleTest() {
    glDisableVertexAttribArray(0);
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
    delete mShader;
    mShader = nullptr;
}

void QuadrangleTest::render()
{
    // 1.clear color buffer
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 2.shader
    mShader->useProgram();

    // 3.draw with vao
    glBindVertexArray(mVAO);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draw with ebo
    glBindVertexArray(0);
}
