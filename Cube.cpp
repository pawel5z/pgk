#include "Cube.hpp"

Cube::Cube() {
    // move cube center to [0, 0, 0]
    for (auto &v : vertices)
        v = (v - Transform::ONE * .5f);
    setBuffers();
    setShaders();
}

void Cube::draw(Camera camera) {}

void Cube::draw(Camera camera, DirectionalLight directionalLight) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    bind();
    glUniformMatrix4fv(0, 1, false, &getModelMat()[0][0]);
    glUniformMatrix4fv(1, 1, false, &camera.getPVMat()[0][0]);
    glUniform3fv(2, 1, &directionalLight.getDir()[0]);
    glUniform3fv(3, 1, &directionalLight.getLightColor()[0]);
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

