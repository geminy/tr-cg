#ifndef QUADRANGLE_H
#define QUADRANGLE_H

#include "AbstractRenderer.h"

class Shader;

class Quadrangle : public AbstractRenderer
{
public:
	Quadrangle();
	~Quadrangle();

	void render() const override;

private:
	Shader* mShader;
	unsigned int mVAO;
	unsigned int mVBO;
	unsigned int mEBO;
};

#endif // !QUADRANGLE_H
