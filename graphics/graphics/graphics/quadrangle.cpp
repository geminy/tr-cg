#include "Quadrangle.h"

#include "Shader.h"

#include <glad/glad.h>

Quadrangle::Quadrangle() {
    // 1.vao
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    // 2.vbo
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    // �ı��εĶ�������
    float vertices[] = {
        // ��һ��������
        0.5f, 0.5f, 0.0f,     // ����
        0.5f, -0.5f, 0.0f,    // ����
        -0.5f, -0.5f, 0.0f,   // ����
        // �ڶ���������
        -0.5f, -0.5f, 0.0f,   // ���£��ظ���
        0.5f, 0.5f, 0.0f,     // ���ϣ��ظ���
        -0.5f, 0.5f, 0.0f     // ����
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3.ebo
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);

    // �ı��ε���������
    unsigned int indices[] = {
        0, 1, 5,              // ��һ��������
        1, 2, 5               // �ڶ���������
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

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // �߿�ģʽ
}

Quadrangle::~Quadrangle() {
    glDisableVertexAttribArray(0);
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
    delete mShader;
    mShader = nullptr;
}

void Quadrangle::render() const {
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mShader->useProgram();

    glBindVertexArray(mVAO);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // ebo
    glBindVertexArray(0);
}
