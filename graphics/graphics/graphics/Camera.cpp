#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

const static float SPEED = 2.5f;
const static float SENSITIVITY = 0.1f;
const static float ZOOM = 60.0f;

Camera::Camera(const glm::vec3& position, const glm::vec3& worldUp, float yaw, float pitch)
	: mPosition(position), mWorldUp(worldUp)
	, mYaw(yaw), mPitch(pitch)
	, mMovementSpeed(SPEED), mMouseSensiticity(SENSITIVITY), mZoom(ZOOM)
	, mForward(glm::vec3(0.0f, 0.0f, -1.0f))
{
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: Camera(glm::vec3(posX, posY, posZ), glm::vec3(upX, upY, upZ), yaw, pitch) {}

Camera::~Camera() {}

glm::vec3 Camera::getPosition() const
{
	return mPosition;
}

glm::vec3 Camera::getWorldUp() const
{
	return mWorldUp;
}

float Camera::getYaw() const
{
	return mYaw;
}

float Camera::getPitch() const
{
	return mPitch;
}

float Camera::getMovementSpeed() const
{
	return mMovementSpeed;
}

float Camera::getMouseSensiticity() const
{
	return mMouseSensiticity;
}

float Camera::getZoom() const
{
	return mZoom;
}

glm::vec3 Camera::getForward() const
{
	return mForward;
}

glm::vec3 Camera::getUp() const
{
	return mUp;
}

glm::vec3 Camera::getRight() const
{
	return mRight;
}

bool Camera::getFlipY() const
{
	return mFlipY;
}

void Camera::setFlipY(bool flip)
{
	mFlipY = flip;
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(mPosition, mPosition + mForward, mUp);
}

void Camera::zoom(float deltaScale)
{
	mZoom += deltaScale;
	if (mZoom > ZOOM) {
		mZoom = ZOOM;
	}
	else if (mZoom < 1.0f) {
		mZoom = 1.0f;
	}
}

void Camera::move(glm::vec3 distance)
{
	mPosition += distance;
}

void Camera::rotate(glm::vec3 rotate)
{
	float yawOffset = mMouseSensiticity * rotate.x;
	float pitchOffset = mMouseSensiticity * rotate.y;

	mYaw += yawOffset;
	mPitch += pitchOffset * (mFlipY ? -1 : 1);

	if (mPitch > 89.0f) {
		mPitch = 89.0f;
	}
	else if (mPitch < -89.0f) {
		mPitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::onMouseMoved(float xOffset, float yOffset)
{
	rotate(glm::vec3(xOffset, yOffset, 1.0f));
}

void Camera::onMouseScrolled(float xOffset, float yOffset)
{
	zoom(yOffset);
}

void Camera::onMovementUp(float deltaTime)
{
	move(mMovementSpeed * deltaTime * mForward);
}

void Camera::onMovementDown(float deltaTime)
{
	move(-mMovementSpeed * deltaTime * mForward);
}

void Camera::onMovementLeft(float deltaTime)
{
	move(-mMovementSpeed * deltaTime * mRight);
}

void Camera::onMovementRight(float deltaTime)
{
	move(mMovementSpeed * deltaTime * mRight);
}

void Camera::onDigitKeyPressed(int digit)
{

}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(mPitch)) * cos(glm::radians(mYaw));
	front.y = sin(glm::radians(mPitch));
	front.z = cos(glm::radians(mPitch)) * sin(glm::radians(mYaw));
	mForward = glm::normalize(front);
	mRight = glm::normalize(glm::cross(mForward, mWorldUp));
	mUp = glm::normalize(glm::cross(mRight, mForward));
}
