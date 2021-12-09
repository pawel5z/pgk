#include "PointLight.hpp"

#include <stdexcept>

PointLight::PointLight(glm::vec3 pos, glm::vec3 color) : pos(pos), color(color) {}

void PointLight::setIntensity(float intensity) {
    if (intensity < 0.f)
        throw std::invalid_argument("intensity out of range");
    this->intensity = intensity;
}

float PointLight::getIntensity() const {
    return intensity;
}

glm::vec3 PointLight::getLightColor() const {
    return color * intensity;
}