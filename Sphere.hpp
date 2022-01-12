#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "AGL3Drawable.hpp"

#include <vector>

// sphere with radius 0.5
class Sphere : public AGLDrawable {
public:
    explicit Sphere(int n=45);
    void draw(Camera camera) override;

protected:
    void setBuffers(std::vector<glm::vec3> &vertices, std::vector<GLushort> &indices);
    void setShaders();

private:
    size_t indicesCnt;

    void initWithPolarCoords(int n, std::vector<glm::vec3> &vertices, std::vector<GLushort> &indices);
};

#endif //SPHERE_HPP
