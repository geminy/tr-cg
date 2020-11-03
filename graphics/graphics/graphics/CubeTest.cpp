#include "CubeTest.h"

#include "Application.h"
#include "Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const static char* VERTEX_PATH = "F:\\evo\\github\\tr-cg\\graphics\\graphics\\graphics\\res\\shader\\Cube.vert";
const static char* FRAGMENT_PATH = "F:\\evo\\github\\tr-cg\\graphics\\graphics\\graphics\\res\\shader\\Cube.frag";

CubeTest::CubeTest() {
    // 1.vao
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    // 2.vbo
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    // ���������飨λ�á���ɫ��
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  1.0f,0.0f,0.0f,
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
    //mShader->enableLineMode(); // �߿�ģʽ
    mShader->enableDepthTest();
}

CubeTest::~CubeTest() {
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    delete mShader;
    mShader = nullptr;
}

void CubeTest::render() {
    // 1.clear buffer
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2.shader
    mShader->useProgram();
    // Transform����任����
    glm::mat4 model(1); // model���󣬾ֲ�����任����������
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f)); // ��ת�Ƕȸ���ʱ��仯
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 view(1); // view������������任���۲�����ϵ
    static glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);     //�����λ��
    static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);       //���������
    static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);           //�����������
    view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    glm::mat4 projection(1); // projection����ͶӰ����
    projection = glm::perspective(glm::radians(45.0f), 1.0f * Application::GetInstance().getWindowWidth() / Application::GetInstance().getWindowHeight(), 0.1f, 100.0f);
    // ����ɫ���д������
    mShader->setUniformMat4("model", model);
    mShader->setUniformMat4("view", view);
    mShader->setUniformMat4("projection", projection);

    // 3.draw with vao
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
