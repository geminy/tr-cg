GLint maxUniformLen;
GLint numUniforms;
char *uniformName;
GLint index;

glGetProgramiv(progObj, GL_ACTIVE_UNIFORMS, &numUniforms);
glGetProgramiv(progObj, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLen);
uniformName = malloc(sizeof(char) * maxUniformLen);

for (index = 0; index < numUniforms; ++i) {
	Glint len;
	GLint size;
	GLenum type;
	GLint location;

    // get the uniform info
	glGetAcitveUniform(progObj, index, maxUniformLen, &len, &size, &type, uniformName);
	// get the uniform location
	location = glGetUniformLocation(progObj, uniformName);
}