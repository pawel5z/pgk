#include "Terrain.hpp"

Terrain::Terrain() {
    GLushort step = 1;
    for (GLushort lod = 0; lod <= maxLOD; lod++) {
        for (GLushort row = 0; row < Terrain::elementsCnt - step; row += step) {
            for (GLushort col = 0; col < Terrain::elementsCnt; col += step) {
                indices[lod].push_back(row * Terrain::elementsCnt + col);
                indices[lod].push_back((row + step) * Terrain::elementsCnt + col);
            }
        }
        step *= 2;
    }
}

void Terrain::draw(Camera camera) {

}
