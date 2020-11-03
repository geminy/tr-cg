#include "Shader.h"

#include <glad/glad.h>

#include <cstdio>
#include <fstream>
#include <sstream>

Shader::Shader(const char* vertex, const char* fragment, bool fromPath)
{
    if (fromPath) {
        createProgramFromPath(vertex, fragment);
    }
    else {
        createProgramFromSource(vertex, fragment);
    }
}

Shader::~Shader()
{
    glDeleteProgram(mProgramId);
}

unsigned int Shader::getProgramId() const
{
    return mProgramId;
}

void Shader::useProgram() const
{
    glUseProgram(mProgramId);
}

void Shader::enableLineMode() const
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Shader::enableCullFace() const
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void Shader::enableDepthTest() const
{
    glEnable(GL_DEPTH_TEST);
}

void Shader::setUniformBool(const std::string& name, bool value) const
{
    setUniformInt(name, value);
}

void Shader::setUniformInt(const std::string& name, int value) const
{
    glUniform1i(getUniform(name), value);
}

void Shader::setUniformFloat(const std::string& name, float value) const
{
    glUniform1f(getUniform(name), value);
}

void Shader::setUniformVec2(const std::string& name, float x, float y) const
{
    glUniform2f(getUniform(name), x, y);
}

void Shader::setUniformVec2(const std::string& name, const glm::vec2& value) const
{
    setUniformVec2(name, value.x, value.y);
}

void Shader::setUniformVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(getUniform(name), x, y, z);
}

void Shader::setUniformVec3(const std::string& name, const glm::vec3& value) const
{
    setUniformVec3(name, value.x, value.y, value.z);
}

void Shader::setUniformVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(getUniform(name), x, y, z, w);
}

void Shader::setUniformVec4(const std::string& name, const glm::vec4& value) const
{
    setUniformVec4(name, value.x, value.y, value.z, value.w);
}

void Shader::setUniformMat2(const std::string& name, const glm::mat2& value) const
{
    glUniformMatrix2fv(getUniform(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::setUniformMat3(const std::string& name, const glm::mat3& value) const
{
    glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::setUniformMat4(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::createProgramFromSource(const char* vertex, const char* fragment)
{
    //printf("Shader createProgramFromSource vertex:\n----------\n%s\n----------\n", vertex);
    //printf("Shader createProgramFromSource fragment:\n----------\n%s\n----------\n", fragment);
    // 1.vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex, nullptr);
    glCompileShader(vertexShader);
    checkStatus(vertexShader, true);

    // 2.fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment, nullptr);
    glCompileShader(fragmentShader);
    checkStatus(fragmentShader, true);

    // 3.shader program
    mProgramId = glCreateProgram();
    glAttachShader(mProgramId, vertexShader);
    glAttachShader(mProgramId, fragmentShader);
    glLinkProgram(mProgramId);
    checkStatus(mProgramId, false);

    // 3.delete
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::createProgramFromPath(const char* vertex, const char* fragment)
{
    printf("Shader createProgramFromPath vertex:%s\n", vertex);
    printf("Shader createProgramFromPath fragment:%s\n", fragment);
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

    createProgramFromSource(vertexStr.str().c_str(), fragmentStr.str().c_str());
}

void Shader::checkStatus(unsigned int id, bool compile) const
{
    int success;
    char infoLog[512];
    if (compile) {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id, 512, nullptr, infoLog);
            printf("Shader checkStatus compile error:%s\n", infoLog);
        }
    }
    else {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id, 512, nullptr, infoLog);
            printf("Shader checkStatus link error:%s\n", infoLog);
        }
    }
}

int Shader::getUniform(const std::string& name) const
{
    int position = glGetUniformLocation(mProgramId, name.c_str());
    if (position == -1) {
        printf("Shader getUniform error:%s\n", name.c_str());
    }
    return position;
}
