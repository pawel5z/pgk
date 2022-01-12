#include "AreaFragment.hpp"
#include "Terrain.hpp"
#include "utils.hpp"

#include <fstream>
#include <stdexcept>
#include <filesystem>

AreaFragment::AreaFragment(std::string filePath) {
    std::string fileName = std::filesystem::path(filePath).filename();
    try {
        lowLa = (GLshort)std::stoi(std::string(fileName, 1, 2)) * (1 - 2 * (GLshort)(fileName[0] == 'S'));
        leftLo = (GLshort)std::stoi(std::string(fileName, 4, 3)) * (1 - 2 * (GLshort)(fileName[3] == 'W'));
    } catch (std::exception &e) {
        fprintf(stderr, "%s\n", e.what());
        throw std::invalid_argument("incorrect file path");
    }

    std::ifstream fin(filePath, std::ios_base::binary);
    int readCnt = fastPow(Terrain::elementsCnt, 2);
    for (int i = 0; i < readCnt; i++) {
        short input = 0;
        fin.get(*((char *)&input + 1));
        fin.get(*(char *)&input);
        vData.push_back({short(i % Terrain::elementsCnt), short(i / Terrain::elementsCnt), input});
    }
    fin.close();

    glGenBuffers(1, &vbo);
    bindVbo();
    glBufferData(GL_ARRAY_BUFFER, (int)(vData.size() * sizeof(VertexData)), vData.data(), GL_STATIC_DRAW);
}

AreaFragment::AreaFragment(const AreaFragment &o) {
    vData = std::vector<VertexData>(o.vData);
    glGenBuffers(1, &vbo);
    bindVbo();
    glBufferData(GL_ARRAY_BUFFER, (int)(vData.size() * sizeof(VertexData)), vData.data(), GL_STATIC_DRAW);
    leftLo = o.leftLo;
    lowLa = o.lowLa;
}

AreaFragment &AreaFragment::operator=(const AreaFragment &o) {
    vData = std::vector<VertexData>(o.vData);
    glGenBuffers(1, &vbo);
    bindVbo();
    glBufferData(GL_ARRAY_BUFFER, (int)(vData.size() * sizeof(VertexData)), vData.data(), GL_STATIC_DRAW);
    leftLo = o.leftLo;
    lowLa = o.lowLa;
    return *this;
}

AreaFragment::~AreaFragment() {
    glDeleteBuffers(1, &vbo);
}

GLshort AreaFragment::getLeftLo() const {
    return leftLo;
}

GLshort AreaFragment::getRightLo() const {
    return leftLo + 1;
}

GLshort AreaFragment::getLowLa() const {
    return lowLa;
}

GLshort AreaFragment::getHighLa() const {
    return lowLa + 1;
}

void AreaFragment::prepareForDrawing() {
    glUniform2f(1, leftLo, lowLa);
    bindVbo();
    glVertexAttribPointer(0, 3, GL_SHORT, false, sizeof(VertexData), nullptr);
}

void AreaFragment::bindVbo() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}
