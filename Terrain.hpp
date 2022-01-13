#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "AGL3Drawable.hpp"
#include "AreaFragment.hpp"

#include <vector>
#include <string>
#include <glm/glm.hpp>

static GLubyte maxLOD = 4;

struct LodGroup {
    GLuint idx;
    GLuint size;
};

class Terrain : AGLDrawable {
public:
    static const GLushort tileLength = 1201;

    explicit Terrain(const std::string& dirPath);
    ~Terrain();
    float getMidLo() const;
    float getMidLa() const;
    void draw(Camera camera) override;
    void draw(Camera camera, bool threeDim);
    GLubyte getLod() const;
    void setLod(GLubyte lod);

private:
    GLuint pId{}; // to delete in destructor
    std::vector<AreaFragment> areaFrags;
    std::vector<GLuint> indices;
    std::vector<LodGroup> lodGroups;
    float midLo, midLa;
    GLubyte lod = 0;
};

#endif //TERRAIN_HPP
