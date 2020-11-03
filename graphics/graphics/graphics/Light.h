#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

#include <string>

class Shader;

class Light
{
public:
    Light();
    Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    virtual ~Light();

    void setAmbient(glm::vec3 ambient);
    void setAmbient(float r, float g, float b);
    void setDiffuse(glm::vec3 diffuse);
    void setDiffuse(float r, float g, float b);
    void setSpecular(glm::vec3 specular);
    void setSpecular(float r, float g, float b);

    void setOn(bool on);

    virtual void draw(const Shader& shader, std::string name) = 0;

public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    bool on;
};

class DirectLight : public Light
{
public:
    DirectLight();
    DirectLight(glm::vec3 direction);
    DirectLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    ~DirectLight();

    void setDirection(glm::vec3 direction);
    void setDirection(float x, float y, float z);

    void draw(const Shader& shader, std::string name) override;

public:
    glm::vec3 direction;
};

class PointLight : public Light
{
public:
    PointLight();
    PointLight(glm::vec3 position);
    PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
            float constant, float linear, float quadratic);
    ~PointLight();

    void setPosition(glm::vec3 position);
    void setPosition(float x, float y, float z);
    void setAttenuationRatio(glm::vec3 attenuationRatio);
    void setAttenuationRatio(float constant, float linear, float quadratic);

    void draw(const Shader& shader, std::string name) override;

public:
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
};

class SpotLight : public Light
{
public:
    SpotLight();
    SpotLight(glm::vec3 position, glm::vec3 direction);
    SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
            float constant, float linear, float quadratic, float cutOff, float outerCutOff);
    ~SpotLight();

    void setPosition(glm::vec3 position);
    void setPosition(float x, float y, float z);
    void setDirection(glm::vec3 direction);
    void setDirection(float x, float y, float z);
    void setAttenuationRatio(glm::vec3 attenuationRatio);
    void setAttenuationRatio(float constant, float linear, float quadratic);
    void setIntensityRatio(glm::vec2 intensityRatio);
    void setIntensityRatio(float cutOff, float outerCutOff);

    void draw(const Shader& shader, std::string name);

public :
    glm::vec3 position;
    glm::vec3 direction;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};

#endif // !LIGHT_H
