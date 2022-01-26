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

private:
    GLuint vArray = 0;
    GLuint vPosBuf = 0;
    GLuint texture = 0;
    GLuint program = 0;
    GLuint verticesCnt;
};

#endif // IMPORTEDASSET_HPP
