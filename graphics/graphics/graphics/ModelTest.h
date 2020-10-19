#ifndef MODEL_TEST_H
#define MODEL_TEST_H

#include "AbstractRenderer.h"

class Shader;
class Model;

class ModelTest : public AbstractRenderer
{
public:
	ModelTest();
	~ModelTest();

	void render() const override;

private:
	Shader* mShader;
	Model* mModel;
};

#endif // !MODEL_TEST_H
