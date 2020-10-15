#include "Shader.h"

#include <glad/glad.h>

#include <cstdio>
#include <fstream>
#include <sstream>

Shader::Shader(const char* vertex, const char* fragment, bool fromPath) {
    if (fromPath) {
        createProgramFromPath(vertex, fragment);
    }
    else {
        createProgramFramSource(vertex, fragment);
    }
}

Shader::~Shader() {
    glDeleteProgram(mProgramId);
}

void Shader::useProgram() {
    glUseProgram(mProgramId);
}

void Shader::createProgramFramSource(const char* vertex, const char* fragment) {
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex, nullptr);
    glCompileShader(vertexShader);
    int vSuccess;
    char vInfoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vSuccess);
    if (!vSuccess) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, vInfoLog);
        printf("Shader createProgramFramSource compile vertex shader error:%s\n", vInfoLog);
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment, nullptr);
    glCompileShader(fragmentShader);
    int fSuccess;
    char fInfoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fSuccess);
    if (!fSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, fInfoLog);
        printf("Shader createProgramFramSource compile fragment shader error:%s\n", fInfoLog);
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
        printf("Shader createProgramFramSource link program error:%s\n", pInfoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::createProgramFromPath(const char* vertex, const char* fragment) {
    std::stringstream vertexStr, fragmentStr;
    std::ifstream vertexFile, fragmentFile;
    vertexFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    fragmentFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    try {
        vertexFile.open(vertex);
        fragmentFile.open(fragment);
        vertexStr << vertexFile.rdbuf();
        fragmentStr << fragmentFile.rdbuf();
        vertexFile.close();
        fragmentFile.close();
    } catch (std::ifstream::failure e) {
        printf("Shader createProgramFromPath error:%s\n", e.what());
    }

    createProgramFramSource(vertexStr.str().c_str(), fragmentStr.str().c_str());
}
