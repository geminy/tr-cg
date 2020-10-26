#ifndef CUBE_TEST_H
#define CUBE_TEST_H

#include "AbstractRenderer.h"

class Shader;

class CubeTest : public AbstractRenderer
{
public:
	CubeTest();
	~CubeTest();

	void render() const override;

private:
	unsigned int mVAO;
	unsigned int mVBO;
	Shader* mShader;
};

#endif // !CUBE_TEST_H
