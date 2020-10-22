#ifndef MODEL_TEST_H
#define MODEL_TEST_H

#include "AbstractRenderer.h"
#include "Camera.h"

class Shader;
class Model;

class ModelTest : public AbstractRenderer
{
public:
	ModelTest();
	~ModelTest();

	void render() const override;

	void onMouseMoved(float xPox, float yPos) override;
	void onMouseScrolled(float xOffset, float yOffset) override;
	void onMovementUp(float deltaTime) override;
	void onMovementDown(float deltaTime) override;
	void onMovementLeft(float deltaTime) override;
	void onMovementRight(float deltaTime) override;

private:
	Camera mCamera;
	Shader* mShader;
	Model* mModel;
};

#endif // !MODEL_TEST_H
