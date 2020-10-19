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
	Shader* mShader;
	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int mEBO;
};

#endif // !SPHERE_TEST_H
