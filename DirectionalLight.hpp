#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class DirectionalLight {
public:
    glm::vec3 color;

    DirectionalLight(glm::vec3 dir, glm::vec3 color);
    void setDir(glm::vec3 dir);
    glm::vec3 getDir();
    void setIntensity(float intensity);
    float getIntensity() const;
    // get color with applied intensity
    glm::vec3 getLightColor() const;

private:
    glm::vec3 dir{};
    float intensity = .5f;
};

#endif //DIRECTIONALLIGHT_HPP
