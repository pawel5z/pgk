#include "Cube.hpp"

Cube::Cube() {
    // move cube center to [0, 0, 0] and make its side length of 1
    for (auto &v : vertices)
        v = (v - Transform::ONE * .5f) * .5f;
    setBuffers();
    setShaders();
}

void Cube::draw(Camera camera) {
    bind();
    glUniformMatrix4fv(0, 1, false, &(camera.getPVMat() * getModelMat())[0][0]);
    glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_BYTE, nullptr);
}

void Cube::setBuffers() {
    bindVertexArray();
    glBufferData(GL_ARRAY_BUFFER, (int)(vertices.size() * sizeof(glm::vec3)), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), nullptr);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)(indices.size() * sizeof(GLubyte)), indices.data(), GL_STATIC_DRAW);
}

void Cube::setShaders() {
    compileShadersFromFile("cubeVS.glsl", "cubeFS.glsl");
}