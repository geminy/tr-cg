#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

const static float SPEED = 2.5f;
const static float SENSITIVITY = 0.1f;
const static float ZOOM = 45.0f;

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: mYaw(yaw), mPitch(pitch)
	, mMovementSpeed(SPEED), mMouseSensiticity(SENSITIVITY), mZoom(ZOOM)
	, mPosition(position)
	, mForward(glm::vec3(0.0f, 0.0f, -1.0f))
	, mWorldUp(up)
{
	updateCameraVectors();
}

Camera::Camera(float posX, float poxY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: mYaw(yaw), mPitch(pitch)
	, mMovementSpeed(SPEED), mMouseSensiticity(SENSITIVITY), mZoom(ZOOM)
	, mPosition(glm::vec3(posX, poxY, posZ))
	, mForward(glm::vec3(0.0f, 0.0f, -1.0f))
	, mWorldUp(glm::vec3(upX, upY, upZ))
{
	updateCameraVectors();
}

Camera::~Camera() {}

float Camera::getZoom() const
{
	return mZoom;
}

glm::vec3 Camera::getPosition() const
{
	return mPosition;
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(mPosition, mPosition + mForward, mUp);
}

void Camera::onMouseMoved(float xOffset, float yOffset)
{
	float yawOffset = mMouseSensiticity * xOffset;
	float pitchOffset = mMouseSensiticity * yOffset;

	mYaw += yawOffset;
	mPitch += pitchOffset;

	if (mPitch > 89.0f) {
		mPitch = 89.0f;
	} else if (mPitch < -89.0f) {
		mPitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::onMouseScrolled(float xOffset, float yOffset)
{
	if (mZoom >= 1.0f && mZoom <= 45.0f) {
		mZoom -= yOffset;
	}

	if (mZoom <= 1.0f) {
		mZoom = 1.0f;
	} else if (mZoom >= 45.0f) {
		mZoom = 45.0f;
	}
}

void Camera::onMovementUp(float deltaTime)
{
	float velocity = mMovementSpeed * deltaTime;
	mPosition += mForward * velocity;

}

void Camera::onMovementDown(float deltaTime)
{
	float velocity = mMovementSpeed * deltaTime;
	mPosition -= mForward * velocity;
}

void Camera::onMovementLeft(float deltaTime)
{
	float velocity = mMovementSpeed * deltaTime;
	mPosition -= mRight * velocity;
}

void Camera::onMovementRight(float deltaTime)
{
	float velocity = mMovementSpeed * deltaTime;
	mPosition += mRight * velocity;
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
