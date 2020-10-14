#ifndef SPHERE_H
#define SPHERE_H

#include "AbstractRenderer.h"

class Shader;

class Sphere : public AbstractRenderer
{
public:
	Sphere();
	~Sphere();

	void render() const override;

private:
	Shader* mShader;
	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int mEBO;
};

#endif // !SPHERE_H
