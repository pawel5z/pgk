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
    // each row defines single wall
    std::vector<glm::vec3> vertices;

    void initWithTetrahedron(int n);
    void initWithPolarCoords(int n);
};

#endif //SPHERE_HPP
