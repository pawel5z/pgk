#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "AGL3Drawable.hpp"
#include "AreaFragment.hpp"

#include <vector>
#include <string>
#include <glm/glm.hpp>

static GLubyte maxLOD = 3;

class Terrain : AGLDrawable {
public:
    static const GLushort elementsCnt = 1201;

    explicit Terrain(const std::string& dirPath);
    float getMidLo() const;
    float getMidLa() const;
    void draw(Camera camera) override;
    void draw(Camera camera, int lod);

private:
    std::vector<AreaFragment> areaFrags;
    std::vector<std::vector<GLushort>> indices;
    float midLo, midLa;
};

#endif //TERRAIN_HPP
