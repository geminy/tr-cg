#include "Light.h"

#include "Shader.h"

const static glm::vec3 DEFAULT_AMBIENT = glm::vec3(0.2f);
const static glm::vec3 DEFAULT_DIFFUSE = glm::vec3(0.5f);
const static glm::vec3 DEFAULT_SPECULAR = glm::vec3(1.0f);
const static glm::vec3 DEFAULT_POSITION = glm::vec3(0.0f);
const static glm::vec3 DEFAULT_DIRECTION = glm::vec3(0.0f, 0.0f, -1.0f);
const static glm::vec3 DEFAULT_ATTENUATION_RATIO = glm::vec3(1.0f, 0.09f, 0.032f);
const static glm::vec2 DEFAULT_INTENSITY_RATIO = glm::vec2(cos(glm::radians(12.5f)), cos(glm::radians(15.0f)));

Light::Light()
    : Light(DEFAULT_AMBIENT, DEFAULT_DIFFUSE, DEFAULT_SPECULAR) {}

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : ambient(ambient), diffuse(diffuse), specular(specular), on(false) {}

Light::~Light() {}

void Light::setAmbient(glm::vec3 ambient)
{
    this->ambient = ambient;
}

void Light::setAmbient(float r, float g, float b)
{
    setAmbient(glm::vec3(r, g, b));
}

void Light::setDiffuse(glm::vec3 diffuse)
{
    this->diffuse = diffuse;
}

void Light::setDiffuse(float r, float g, float b)
{
    setDiffuse(glm::vec3(r, g, b));
}

void Light::setSpecular(glm::vec3 specular)
{
    this->specular = specular;
}

void Light::setSpecular(float r, float g, float b)
{
    setSpecular(glm::vec3(r, g, b));
}

void Light::setOn(bool on)
{
    this->on = on;
}

DirectLight::DirectLight()
    : DirectLight(DEFAULT_DIRECTION) {}

DirectLight::DirectLight(glm::vec3 direction)
    : DirectLight(direction, DEFAULT_AMBIENT, DEFAULT_DIFFUSE, DEFAULT_SPECULAR) {}

DirectLight::DirectLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : Light(ambient, diffuse, specular), direction(direction) {}

DirectLight::~DirectLight() {}

void DirectLight::setDirection(glm::vec3 direction)
{
    this->direction = direction;
}

void DirectLight::setDirection(float x, float y, float z)
{
    setDirection(glm::vec3(x, y, z));
}

void DirectLight::draw(const Shader& shader, std::string name)
{
    shader.setUniformBool(name + ".on", on);
    shader.setUniformVec3(name + ".direction", direction);
    shader.setUniformVec3(name + ".ambient", ambient);
    shader.setUniformVec3(name + ".diffuse", diffuse);
    shader.setUniformVec3(name + ".specular", specular);
}

PointLight::PointLight()
    : PointLight(DEFAULT_POSITION) {}

PointLight::PointLight(glm::vec3 position) 
    : PointLight(position, DEFAULT_AMBIENT, DEFAULT_DIFFUSE, DEFAULT_SPECULAR) {}

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : PointLight(position, ambient, diffuse, specular, DEFAULT_ATTENUATION_RATIO.x, DEFAULT_ATTENUATION_RATIO.y, DEFAULT_ATTENUATION_RATIO.z) {}

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
    : Light(ambient, diffuse, specular), position(position), constant(constant), linear(linear), quadratic(quadratic) {}

PointLight::~PointLight() {}

void PointLight::setPosition(glm::vec3 position)
{
    this->position = position;
}

void PointLight::setPosition(float x, float y, float z)
{
    setPosition(glm::vec3(x, y, z));
}

void PointLight::setAttenuationRatio(float constant, float linear, float quadratic)
{
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
}

void PointLight::setAttenuationRatio(glm::vec3 attenuationRatio)
{
    setAttenuationRatio(attenuationRatio.x, attenuationRatio.y, attenuationRatio.z);
}

void PointLight::draw(const Shader& shader, std::string name)
{
    shader.setUniformBool(name + ".on", on);
    shader.setUniformVec3(name + ".position", position);
    shader.setUniformVec3(name + ".ambient", ambient);
    shader.setUniformVec3(name + ".diffuse", diffuse);
    shader.setUniformVec3(name + ".specular", specular);
    shader.setUniformFloat(name + ".c", constant);
    shader.setUniformFloat(name + ".l", linear);
    shader.setUniformFloat(name + ".q", quadratic);
}

SpotLight::SpotLight() : SpotLight(DEFAULT_POSITION, DEFAULT_DIRECTION) {}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction) 
    : SpotLight(position, direction, DEFAULT_AMBIENT, DEFAULT_DIFFUSE, DEFAULT_SPECULAR) {}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : SpotLight(position, direction, ambient, diffuse, specular
    , DEFAULT_ATTENUATION_RATIO.x, DEFAULT_ATTENUATION_RATIO.y, DEFAULT_ATTENUATION_RATIO.z
    , DEFAULT_INTENSITY_RATIO.x, DEFAULT_INTENSITY_RATIO.y) {}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff)
    : Light(ambient, diffuse, specular)
    , position(position), direction(direction)
    , constant(constant), linear(linear), quadratic(quadratic)
    , cutOff(cutOff), outerCutOff(outerCutOff) {}

SpotLight::~SpotLight() {}

void SpotLight::setPosition(glm::vec3 position)
{
    this->position = position;
}

void SpotLight::setPosition(float x, float y, float z)
{
    setPosition(glm::vec3(x, y, z));
}

void SpotLight::setDirection(glm::vec3 direction)
{
    this->direction = direction;
}

void SpotLight::setDirection(float x, float y, float z)
{
    setDirection(glm::vec3(x, y, z));
}

void SpotLight::setAttenuationRatio(float constant, float linear, float quadratic)
{
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
}

void SpotLight::setAttenuationRatio(glm::vec3 attenuationRatio)
{
    setAttenuationRatio(attenuationRatio.x, attenuationRatio.y, attenuationRatio.z);
}

void SpotLight::setIntensityRatio(float cutOff, float outerCutOff)
{
    this->cutOff = cutOff;
    this->outerCutOff = outerCutOff;
}

void SpotLight::setIntensityRatio(glm::vec2 intensityRatio)
{
    setIntensityRatio(intensityRatio.x, intensityRatio.y);
}

void SpotLight::draw(const Shader& shader, std::string name)
{
    shader.setUniformBool(name + ".on", on);
    shader.setUniformVec3(name + ".position", position);
    shader.setUniformVec3(name + ".direction", direction);
    shader.setUniformVec3(name + ".ambient", ambient);
    shader.setUniformVec3(name + ".diffuse", diffuse);
    shader.setUniformVec3(name + ".specular", specular);
    shader.setUniformFloat(name + ".c", constant);
    shader.setUniformFloat(name + ".l", linear);
    shader.setUniformFloat(name + ".q", quadratic);
    shader.setUniformFloat(name + ".cutOff", cutOff);
    shader.setUniformFloat(name + ".outerCutOff", outerCutOff);
}
