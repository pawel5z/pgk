#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class PointLight {
public:
    glm::vec3 pos;
    glm::vec3 color;

    PointLight(glm::vec3 pos, glm::vec3 color);
    void setIntensity(float intensity);
    float getIntensity() const;
    // get color with applied intensity
    glm::vec3 getLightColor() const;

private:
    float intensity = .5f;
};

#endif //POINTLIGHT_HPP
