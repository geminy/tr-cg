#ifndef TRIANGLE_TEST_H
#define TRIANGLE_TEST_H

#include "AbstractRenderer.h"

class Shader;

class TriangleTest : public AbstractRenderer
{
public:
	TriangleTest();
	~TriangleTest();

	void render() const override;

private:
	Shader* mShader;
	unsigned int mVAO;
	unsigned int mVBO;
};

#endif // !TRIANGLE_TEST_H
