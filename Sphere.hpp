#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "AGL3Drawable.hpp"

#include <vector>

// sphere with radius 0.5
class Sphere : public AGLDrawable {
public:
    explicit Sphere(int n = 3);
    void draw(Camera camera) override;

protected:
    void setBuffers();
    void setShaders();

private:
    std::vector<glm::vec3> vertices;
    std::vector<GLushort> indices;

    void initWithPolarCoords(int n);
};

#endif //SPHERE_HPP
