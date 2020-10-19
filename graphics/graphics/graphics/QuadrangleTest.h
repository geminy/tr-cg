#ifndef QUADRANGLE_TEST_H
#define QUADRANGLE_TEST_H

#include "AbstractRenderer.h"

class Shader;

class QuadrangleTest : public AbstractRenderer
{
public:
	QuadrangleTest();
	~QuadrangleTest();

	void render() const override;

private:
	Shader* mShader;
	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int mEBO;
};

#endif // !QUADRANGLE_TEST_H
