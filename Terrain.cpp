#include "Terrain.hpp"
#include "utils.hpp"

#include <filesystem>
#include <stdexcept>

Terrain::Terrain(const std::string& dirPath) {
    try {
        for (const auto &dirEntry : std::filesystem::directory_iterator(dirPath)) {
            if (!dirEntry.is_regular_file())
                continue;
            try {
                areaFrags.emplace_back(dirEntry.path());
            } catch (std::invalid_argument &e) {
                fprintf(stderr, "%s\n", e.what());
                throw std::invalid_argument("incorrect file name in specified path");
            }
        }
    } catch (std::exception &e) {
        fprintf(stderr, "%s\n", e.what());
        throw std::logic_error("error while reading " + dirPath + " directory");
    }

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

    GLushort step = 1;
    for (GLushort lod = 0; lod <= maxLOD; lod++) {
        indices.emplace_back();
        for (GLushort row = 0; row < Terrain::elementsCnt - step; row += step) {
            for (GLushort col = 0; col < Terrain::elementsCnt; col += step) {
                indices.at(lod).push_back(row * Terrain::elementsCnt + col);
                indices.at(lod).push_back((row + step) * Terrain::elementsCnt + col);
            }
        }
        step *= 2;
    }

    compileShadersFromFile("areaMapVS.glsl", "areaFS.glsl");
    glEnableVertexAttribArray(0);
}

float Terrain::getMidLo() const {
    return midLo;
}

float Terrain::getMidLa() const {
    return midLa;
}

void Terrain::draw(Camera camera) {
    throw std::logic_error("not implemented");
}

void Terrain::draw(Camera camera, int lod) {
    bind();
    // eboId already bound
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)(indices[lod].size() * sizeof(GLushort)), indices[lod].data(), GL_STATIC_DRAW);
    glUniformMatrix4fv(0, 1, false, &(camera.getPVMat() * getModelMat())[0][0]);
    for (auto &area : areaFrags) {
        glUniform2f(1, area.getLeftLo(), area.getLowLa());
        int verticesCntInStrip = ((Terrain::elementsCnt - 1) / fastPow(2, lod) + 1) * 2;
        area.prepareForDrawing();
        for (size_t i = 0; i < indices.at(lod).size(); i += verticesCntInStrip) {
            glDrawElements(GL_TRIANGLE_STRIP, verticesCntInStrip, GL_UNSIGNED_SHORT, (void *)i);
        }
    }
}
