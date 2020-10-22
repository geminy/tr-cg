#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.f, float pitch = 0.0f);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    ~Camera();

    float getZoom() const;
    glm::vec3 getPosition() const;
    glm::mat4 getViewMatrix() const;

    void onMouseMoved(float xOffset, float yOffset);
    void onMouseScrolled(float xOffset, float yOffset);
    void onMovementUp(float deltaTime);
    void onMovementDown(float deltaTime);
    void onMovementLeft(float deltaTime);
    void onMovementRight(float deltaTime);

private:
    void updateCameraVectors();

private:
    float mYaw;
    float mPitch;
    float mMovementSpeed;
    float mMouseSensiticity;
    float mZoom;
    glm::vec3 mPosition;
    glm::vec3 mForward;
    glm::vec3 mWorldUp;
    glm::vec3 mRight;
    glm::vec3 mUp;
};

#endif // !CAMERA_H
