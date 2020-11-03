#ifndef PHONE_TEST_H
#define PHONE_TEST_H

#include "AbstractRenderer.h"
#include "Camera.h"
#include "Light.h"

class Shader;

// Phong
class PhoneTest : public AbstractRenderer
{
public:
	PhoneTest();
	~PhoneTest();

	void render() override;

	void onMouseMoved(float xPox, float yPos, float xOffset, float yOffset) override;
	void onMouseScrolled(float xOffset, float yOffset) override;
	void onMovementUp(float deltaTime) override;
	void onMovementDown(float deltaTime) override;
	void onMovementLeft(float deltaTime) override;
	void onMovementRight(float deltaTime) override;
	void onDigitKeyPressed(int digit) override;

private:
	unsigned int mVAOCube;
	unsigned int mVAOLamp;
	unsigned int mVBO;
	Camera mCamera;
	DirectLight mDirectLight;
	PointLight mPointLights[6];
	SpotLight mSpotLight;
	Shader* mShaderCube;
	Shader* mShaderLamp;
};

#endif // !PHONE_TEST_H
