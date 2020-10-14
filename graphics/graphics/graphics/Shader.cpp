#include "Shader.h"

#include <glad/glad.h>

#include <cstdio>

Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource) {
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    int vSuccess;
    char vInfoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vSuccess);
    if (!vSuccess) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, vInfoLog);
        printf("Shader compile vertex shader error:%s\n", vInfoLog);
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    int fSuccess;
    char fInfoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fSuccess);
    if (!fSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, fInfoLog);
        printf("Shader compile fragment shader error:%s\n", fInfoLog);
    }

    mProgramId = glCreateProgram();
    glAttachShader(mProgramId, vertexShader);
    glAttachShader(mProgramId, fragmentShader);
    glLinkProgram(mProgramId);
    int pSuccess;
    char pInfoLog[512];
    glGetProgramiv(mProgramId, GL_LINK_STATUS, &pSuccess);
    if (!pSuccess) {
        glGetProgramInfoLog(mProgramId, 512, nullptr, pInfoLog);
        printf("Shader link program error:%s\n", pInfoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(mProgramId);
}

void Shader::useProgram() {
    glUseProgram(mProgramId);
}
