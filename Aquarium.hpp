#ifndef AQUARIUM_HPP
#define AQUARIUM_HPP

#include "AGL3Drawable.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"

// cube with dimensions 1x1x1
class Aquarium : public AGLDrawable {
public:
    Aquarium();
    ~Aquarium();
    void draw(Camera camera) override;
    void draw(Camera camera, DirectionalLight directionalLight, PointLight pointLight);

private:
    GLuint normVBO{};
    std::vector<glm::vec3> vertices = {
            // bottom
            {0, 0, 0}, {1, 0, 1}, {0, 0, 1},
            {0, 0, 0}, {1, 0, 0}, {1, 0, 1},
            // back
            {0, 0, 0}, {1, 1, 0}, {1, 0, 0},
            {0, 0, 0}, {0, 1, 0}, {1, 1, 0},
            // right
            {0, 0, 0}, {0, 0, 1}, {0, 1, 1},
            {0, 0, 0}, {0, 1, 1}, {0, 1, 0},
            // up
            {1, 1, 1}, {1, 1, 0}, {0, 1, 0},
            {1, 1, 1}, {0, 1, 0}, {0, 1, 1},
            // left
            {1, 1, 1}, {1, 0, 0}, {1, 1, 0},
            {1, 1, 1}, {1, 0, 1}, {1, 0, 0},
            // front
            {1, 1, 1}, {0, 1, 1}, {0, 0, 1},
            {1, 1, 1}, {0, 0, 1}, {1, 0, 1},
    };
    std::vector<glm::vec3> normals = {
            // bottom
            Transform::UP, Transform::UP, Transform::UP,
            Transform::UP, Transform::UP, Transform::UP,
            // back
            Transform::FORWARD, Transform::FORWARD, Transform::FORWARD,
            Transform::FORWARD, Transform::FORWARD, Transform::FORWARD,
            // right
            Transform::LEFT, Transform::LEFT, Transform::LEFT,
            Transform::LEFT, Transform::LEFT, Transform::LEFT,
            // up
            Transform::UP, Transform::UP, Transform::UP,
            Transform::UP, Transform::UP, Transform::UP,
            // left
            Transform::RIGHT, Transform::RIGHT, Transform::RIGHT,
            Transform::RIGHT, Transform::RIGHT, Transform::RIGHT,
            // front
            Transform::BACKWARD, Transform::BACKWARD, Transform::BACKWARD,
            Transform::BACKWARD, Transform::BACKWARD, Transform::BACKWARD
    };

    void setBuffers();
    void setShaders();
};

#endif //AQUARIUM_HPP
