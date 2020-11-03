#include "CameraTest.h"

#include "Application.h"
#include "Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const static char* VERTEX_PATH = "F:\\evo\\github\\tr-cg\\graphics\\graphics\\graphics\\res\\shader\\Camera.vert";
const static char* FRAGMENT_PATH = "F:\\evo\\github\\tr-cg\\graphics\\graphics\\graphics\\res\\shader\\Camera.frag";

CameraTest::CameraTest()
    : mCamera(glm::vec3(0.0f, 0.0f, 3.0f))
{
    // 1.vao
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    // 2.vbo
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,0.0f,0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,0.0f,0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,0.0f,0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f,0.0f,0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f,0.0f,0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,1.0f,0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,1.0f,0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,1.0f,0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,1.0f,0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,1.0f,0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,1.0f,0.0f,

        -0.5f,  0.5f,  0.5f,  0.0f,0.0f,1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,0.0f,1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,0.0f,1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,0.0f,1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,0.0f,1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,0.0f,1.0f,

        0.5f,  0.5f,  0.5f,  0.5f,0.0f,0.0f,
        0.5f,  0.5f, -0.5f,  0.5f,0.0f,0.0f,
        0.5f, -0.5f, -0.5f,  0.5f,0.0f,0.0f,
        0.5f, -0.5f, -0.5f,  0.5f,0.0f,0.0f,
        0.5f, -0.5f,  0.5f,  0.5f,0.0f,0.0f,
        0.5f,  0.5f,  0.5f,  0.5f,0.0f,0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f,0.5f,0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,0.5f,0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,0.5f,0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,0.5f,0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,0.5f,0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,0.5f,0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,0.0f,0.5f,
        0.5f,  0.5f, -0.5f,  0.0f,0.0f,0.5f,
        0.5f,  0.5f,  0.5f,  0.0f,0.0f,0.5f,
        0.5f,  0.5f,  0.5f,  0.0f,0.0f,0.5f,
        -0.5f,  0.5f,  0.5f,  0.0f,0.0f,0.5f,
        -0.5f,  0.5f, -0.5f,  0.0f,0.0f,0.5f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3.vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 4.unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 5.shader
    mShader = new Shader(VERTEX_PATH, FRAGMENT_PATH, true);
    glEnable(GL_DEPTH_TEST);
}

CameraTest::~CameraTest() {
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    delete mShader;
    mShader = nullptr;
}

void CameraTest::render() {
    // 1.clear buffer
    glClearColor(0.0f, 0.34f, 0.57f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2.shader
    mShader->useProgram();
    glm::mat4 model(1);
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    glm::mat4 view(1);
    view = mCamera.getViewMatrix();
    glm::mat4 projection(1);
    projection = glm::perspective(glm::radians(mCamera.getZoom()), 1.0f * Application::GetInstance().getWindowWidth() / Application::GetInstance().getWindowHeight(), 0.1f, 100.0f);
    mShader->setUniformMat4("model", model);
    mShader->setUniformMat4("view", view);
    mShader->setUniformMat4("projection", projection);

    // 3.vao draw
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void CameraTest::onMouseMoved(float xPos, float yPos, float xOffset, float yOffset)
{
    mCamera.onMouseMoved(xOffset, yOffset);
}

void CameraTest::onMouseScrolled(float xOffset, float yOffset)
{
    mCamera.onMouseScrolled(xOffset, yOffset);
}

void CameraTest::onMovementUp(float deltaTime)
{
    mCamera.onMovementUp(deltaTime);
}

void CameraTest::onMovementDown(float deltaTime)
{
    mCamera.onMovementDown(deltaTime);
}

void CameraTest::onMovementLeft(float deltaTime)
{
    mCamera.onMovementLeft(deltaTime);
}

void CameraTest::onMovementRight(float deltaTime)
{
    mCamera.onMovementRight(deltaTime);
}
