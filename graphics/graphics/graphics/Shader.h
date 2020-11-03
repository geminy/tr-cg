#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>

#include <string>

class Shader
{
public:
	Shader(const char* vertex, const char* fragment, bool fromPath = false);
	~Shader();

    unsigned int getProgramId() const;
	void useProgram() const;

    void enableLineMode() const;
    void enableCullFace() const;
    void enableDepthTest() const;

    void setUniformBool(const std::string& name, bool value) const;
    void setUniformInt(const std::string& name, int value) const;
    void setUniformFloat(const std::string& name, float value) const;
    void setUniformVec2(const std::string& name, const glm::vec2& value) const;
    void setUniformVec2(const std::string& name, float x, float y) const;
    void setUniformVec3(const std::string& name, const glm::vec3& value) const;
    void setUniformVec3(const std::string& name, float x, float y, float z) const;
    void setUniformVec4(const std::string& name, const glm::vec4& value) const;
    void setUniformVec4(const std::string& name, float x, float y, float z, float w) const;
    void setUniformMat2(const std::string& name, const glm::mat2& value) const;
    void setUniformMat3(const std::string& name, const glm::mat3& value) const;
    void setUniformMat4(const std::string& name, const glm::mat4& value) const;

private:
	void createProgramFromSource(const char* vertex, const char* fragment);
	void createProgramFromPath(const char* vertex, const char* fragment);
    void checkStatus(unsigned int id, bool compile) const;
    int getUniform(const std::string& name) const;

private:
	unsigned int mProgramId;
};

#endif // !SHADER_H
