#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "AGL3Drawable.hpp"

#include <vector>

// sphere with radius 0.5
class Sphere : public AGLDrawable {
public:
    Sphere(int stacks, int sectors);
    void draw(Camera camera) override;

protected:
    void setBuffers(std::vector<glm::vec3> &vertices, std::vector<GLuint> &indices);
    void setShaders();

private:
    int stacks, sectors;
    size_t indicesCnt;

    static float latitudeAng(int i, int stacks);
    static float longitudeAng(int i, int sectors);
    static void initWithPolarCoords(int stacks, int sectors, std::vector<glm::vec3> &vertices, std::vector<GLuint> &indices);
};

#endif //SPHERE_HPP
