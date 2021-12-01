#ifndef CUBE_HPP
#define CUBE_HPP

#include "AGL3Drawable.hpp"

// cube with dimensions 1x1x1
class Cube : public AGLDrawable {
public:
    Cube();
    void draw(Camera camera) override;

private:
    std::vector<glm::vec3> vertices = {
            {0, 0, 0}, // 0
            {0, 0, 1}, // 1
            {0, 1, 0}, // 2
            {0, 1, 1}, // 3
            {1, 0, 0}, // 4
            {1, 0, 1}, // 5
            {1, 1, 0}, // 6
            {1, 1, 1}  // 7
    };
    std::vector<GLubyte> indices = {
        0, 5, 1,
        0, 5, 4,
        0, 6, 4,
        0, 6, 2,
        0, 3, 1,
        0, 3, 2,
        7, 2, 6,
        7, 2, 3,
        7, 4, 6,
        7, 4, 5,
        7, 1, 3,
        7, 1, 5
    };

    void setBuffers();
    void setShaders();
};

#endif //LAB4_CUBE_HPP
