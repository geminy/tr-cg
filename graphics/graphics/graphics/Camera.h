#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& worldUp = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.f, float pitch = 0.0f);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    ~Camera();

    glm::vec3 getPosition() const;
    glm::vec3 getWorldUp() const;
    float getYaw() const;
    float getPitch() const;

    float getMovementSpeed() const;
    float getMouseSensiticity() const;
    float getZoom() const;
    glm::vec3 getForward() const;
    glm::vec3 getUp() const;
    glm::vec3 getRight() const;

    bool getFlipY() const;
    void setFlipY(bool flip);

    glm::mat4 getViewMatrix() const;

    void onMouseMoved(float xOffset, float yOffset);
    void onMouseScrolled(float xOffset, float yOffset);

    void onMovementUp(float deltaTime);
    void onMovementDown(float deltaTime);
    void onMovementLeft(float deltaTime);
    void onMovementRight(float deltaTime);

    void onDigitKeyPressed(int digit);

    void zoom(float scale);
    void move(glm::vec3 distance);
    void rotate(glm::vec3 rotate);

private:
    void updateCameraVectors();

private:
    glm::vec3 mPosition;
    glm::vec3 mWorldUp;
    float mYaw;
    float mPitch;

    float mMovementSpeed;
    float mMouseSensiticity;
    float mZoom;
    glm::vec3 mForward;
    glm::vec3 mRight;
    glm::vec3 mUp;

    bool mFlipY = false;;
};

#endif // !CAMERA_H
