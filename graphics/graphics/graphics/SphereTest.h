#ifndef SPHERE_TEST_H
#define SPHERE_TEST_H

#include "AbstractRenderer.h"

class Shader;

class SphereTest : public AbstractRenderer
{
public:
	SphereTest();
	~SphereTest();

	void render() const override;

private:
	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int mEBO;
	Shader* mShader;
};

#endif // !SPHERE_TEST_H
