#ifndef SHADER_H
#define SHADER_H

class Shader
{
public:
	Shader(const char* vertex, const char* fragment, bool fromPath = false);
	~Shader();

	void useProgram();

private:
	void createProgramFramSource(const char* vertex, const char* fragment);
	void createProgramFromPath(const char* vertex, const char* fragment);

private:
	unsigned int mProgramId;
};

#endif // !SHADER_H
