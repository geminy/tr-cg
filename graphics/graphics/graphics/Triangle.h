#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "AbstractRenderer.h"

class Shader;

class Triangle : public AbstractRenderer
{
public:
	Triangle();
	~Triangle();

	void render() const override;

private:
	Shader* mShader;
	unsigned int mVAO;
	unsigned int mVBO;
};

#endif // !TRIANGLE_H
