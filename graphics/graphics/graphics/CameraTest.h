#ifndef CAMERA_TEST_H
#define CAMERA_TEST_H

#include "AbstractRenderer.h"
#include "Camera.h"

class Shader;

class CameraTest : public AbstractRenderer
{
public:
	CameraTest();
	~CameraTest();

	void render() const override;

	void onMouseMoved(float xPox, float yPos, float xOffset, float yOffset) override;
	void onMouseScrolled(float xOffset, float yOffset) override;
	void onMovementUp(float deltaTime) override;
	void onMovementDown(float deltaTime) override;
	void onMovementLeft(float deltaTime) override;
	void onMovementRight(float deltaTime) override;

private:
	unsigned int mVAO;
	unsigned int mVBO;
	Camera mCamera;
	Shader* mShader;
};

#endif // !CAMERA_TEST_H
