#ifndef IMPORTEDASSET_HPP
#define IMPORTEDASSET_HPP

#include "Camera.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <epoxy/gl.h>
#include <epoxy/glx.h>

class ImportedAsset : Transform {
public:
    ImportedAsset(std::string path, std::string vertPath, std::string fragPath);
    ImportedAsset(std::string path, std::string vertPath, std::string fragPath, std::string texPath);
    ~ImportedAsset();
    void draw(const Camera &camera);
    glm::vec3 getCenter() const;
    float getGreatestZ() const;

private:
    GLuint vArray = 0;
    GLuint vPosBuf = 0;
    GLuint texture = 0;
    GLuint program = 0;
    GLuint verticesCnt;
    glm::vec3 center = {0.f, 0.f, 0.f};
    float greatestZ = 0.f;
};

#endif // IMPORTEDASSET_HPP
