#ifndef TRIANGLE_TEST_H
#define TRIANGLE_TEST_H

#include "AbstractRenderer.h"

class Shader;

class TriangleTest : public AbstractRenderer
{
public:
	TriangleTest();
	~TriangleTest();

	void render() override;

private:
	unsigned int mVAO;
	unsigned int mVBO;
	Shader* mShader;
};

#endif // !TRIANGLE_TEST_H
