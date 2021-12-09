#ifndef CUBE_HPP
#define CUBE_HPP

#include "AGL3Drawable.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"

// cube with dimensions 1x1x1
class Cube : public AGLDrawable {
public:
    Cube();
    ~Cube();
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
            Transform::DOWN, Transform::DOWN, Transform::DOWN,
            Transform::DOWN, Transform::DOWN, Transform::DOWN,
            // back
            Transform::BACKWARD, Transform::BACKWARD, Transform::BACKWARD,
            Transform::BACKWARD, Transform::BACKWARD, Transform::BACKWARD,
            // right
            Transform::RIGHT, Transform::RIGHT, Transform::RIGHT,
            Transform::RIGHT, Transform::RIGHT, Transform::RIGHT,
            // up
            Transform::UP, Transform::UP, Transform::UP,
            Transform::UP, Transform::UP, Transform::UP,
            // left
            Transform::LEFT, Transform::LEFT, Transform::LEFT,
            Transform::LEFT, Transform::LEFT, Transform::LEFT,
            // front
            Transform::FORWARD, Transform::FORWARD, Transform::FORWARD,
            Transform::FORWARD, Transform::FORWARD, Transform::FORWARD
    };

    void setBuffers();
    void setShaders();
};

#endif //CUBE_HPP
