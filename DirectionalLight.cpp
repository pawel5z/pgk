#include "DirectionalLight.hpp"

#include <stdexcept>

DirectionalLight::DirectionalLight(glm::vec3 dir, glm::vec3 color) : color(color) {
    setDir(dir);
}

void DirectionalLight::setDir(glm::vec3 dir) {
    if (glm::length2(dir) == 0.f)
        throw std::invalid_argument("dir must not have length of zero");
    this->dir = glm::normalize(dir);
}

glm::vec3 DirectionalLight::getDir() {
    return dir;
}

void DirectionalLight::setIntensity(float intensity) {
    if (intensity < 0.f)
        throw std::invalid_argument("intensity out of range");
    this->intensity = intensity;
}

float DirectionalLight::getIntensity() const {
    return intensity;
}

glm::vec3 DirectionalLight::getLightColor() const {
    return color * intensity;
}
