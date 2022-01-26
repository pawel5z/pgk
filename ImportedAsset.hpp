#ifndef IMPORTEDASSET_HPP

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
    ~ImportedAsset();
    void draw(const Camera &camera);

private:
    GLuint vArray;
    GLuint vPosBuf;
    GLuint program;
    GLuint verticesCnt;
};

#endif // IMPORTEDASSET_HPP
