GLuint blockId, bufferId;
GLint blockSize;
GLuint bindingPoint = 1;
GLfloat lightData[] =
{
	// lightDirection (padded to vec4 based on std140 rule)
	1.0f, 0.0f, 0.0f, 0.0f,
	// lightPosition
	0.0f, 0.0f, 0.0f, 1.0f
}

// retrive the uniform block index
blockId = glGetUniformBlockIndex(program, "LightBlock");
// associate the uniform block index with a binding point
glUniformBlockBinding(program, blockId, bindingPoint);
// get the size of lightData
// alternatively we can calculate it using sizeof(lightData) in this example
glGetAcitveUniformBlockiv(program, blockId, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
// create and fill a buffer object
glGenBuffers(1, &bufferId);
glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
glBufferData(GL_UNIFORM_BUFFER, blockSize, lightData, GL_DYNAMIC_DRAW);
// bind the buffer object to the uniform block bindint point
glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, buffer);