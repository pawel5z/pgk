#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "AGL3Drawable.hpp"

#include <vector>

class Sphere : public AGLDrawable {
public:
    Sphere(int n = 3, bool polar = true);
    void draw(Camera camera) override;

protected:
    void setBuffers();
    void setShaders();

private:
    std::vector<glm::vec3> vertices;
    std::vector<GLushort> indices;

    void initWithTetrahedron(int n);
    void initWithPolarCoords(int n);
};

#endif //SPHERE_HPP
