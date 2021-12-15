#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "AGL3Drawable.hpp"

#include <vector>
#include <glm/glm.hpp>

static GLubyte maxLOD = 3;

class Terrain : AGLDrawable {
public:
    static const GLushort elementsCnt = 1021;

    Terrain();

private:
    std::vector<std::vector<GLushort>> indices;

    void draw(Camera camera) override;
};

#endif //TERRAIN_HPP
