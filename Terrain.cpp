#include "Terrain.hpp"
#include "utils.hpp"

#include <filesystem>
#include <stdexcept>

Terrain::Terrain(const std::string& dirPath) {
    glEnableVertexAttribArray(0);
    compileShadersFromFile("areaMapVS.glsl", "areaFS.glsl");

    GLint step = 1;
    for (GLuint lod = 0; lod <= maxLOD; lod++) {
        lodGroups.push_back({(GLuint)indices.size(), 0});
        for (GLuint row = 0; row < Terrain::elementsCnt - step; row += step) {
            for (GLuint col = 0; col < Terrain::elementsCnt; col += step) {
                indices.push_back(row * Terrain::elementsCnt + col);
                indices.push_back((row + step) * Terrain::elementsCnt + col);
            }
            indices.push_back(primitiveRestartIndex);
        }
        lodGroups.at(lod).size = indices.size() - lodGroups.at(lod).idx;
        step *= 2;
    }
    // eboId already bound
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(indices.size() * sizeof(GLuint)), indices.data(), GL_STATIC_DRAW);

    try {
        for (const auto &dirEntry : std::filesystem::directory_iterator(dirPath)) {
            if (!dirEntry.is_regular_file())
                continue;
            try {
                areaFrags.emplace_back(dirEntry.path());
            } catch (std::invalid_argument &e) {
                fprintf(stderr, "%s\nSkipping %s.\n", e.what(), dirEntry.path().c_str());
            }
        }
    } catch (std::exception &e) {
        fprintf(stderr, "%s\n", e.what());
        throw std::logic_error("error while reading " + dirPath + " directory");
    }
    printf("Rendering %lu tiles.\n", areaFrags.size());

    float minLo = std::numeric_limits<float>::max(), maxLo = std::numeric_limits<float>::min(),
          minLa = std::numeric_limits<float>::max(), maxLa = std::numeric_limits<float>::min();
    for (auto &area : areaFrags) {
        minLo = std::min(minLo, (float)area.getLeftLo());
        maxLo = std::max(maxLo, (float)area.getRightLo());
        minLa = std::min(minLa, (float)area.getLowLa());
        maxLa = std::max(maxLa, (float)area.getHighLa());
    }
    midLo = (minLo + maxLo) / 2.f;
    midLa = (minLa + maxLa) / 2.f;
}

float Terrain::getMidLo() const {
    return midLo;
}

float Terrain::getMidLa() const {
    return midLa;
}

void Terrain::draw(Camera camera) {
    bind();
    glUniformMatrix4fv(0, 1, false, &(camera.getPVMat() * getModelMat())[0][0]);
    for (auto &area : areaFrags) {
        area.prepareForDrawing();
        glDrawElements(GL_TRIANGLE_STRIP, lodGroups[lod].size, GL_UNSIGNED_INT, (void *)(size_t)(lodGroups[lod].idx * sizeof(GLuint)));
    }
}

GLubyte Terrain::getLod() const {
    return lod;
}

void Terrain::setLod(GLubyte lod) {
    if ((size_t)lod >= lodGroups.size())
        throw std::invalid_argument("Unavailable lod. Available ranges: 0 (auto), 1 - " + std::to_string(maxLOD));
    Terrain::lod = lod;
}
