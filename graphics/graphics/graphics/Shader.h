#ifndef SHADER_H
#define SHADER_H

class Shader
{
public:
	Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
	~Shader();

	void useProgram();

private:
	unsigned int mProgramId;
};

#endif // !SHADER_H
