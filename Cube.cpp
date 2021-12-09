#include "Cube.hpp"

Cube::Cube() {
    // move cube center to [0, 0, 0]
    for (auto &v : vertices)
        v = (v - Transform::ONE * .5f);
    glGenBuffers(1, &normVBO);
    setBuffers();
    setShaders();
}

void Cube::draw(Camera camera) {}

void Cube::draw(Camera camera, DirectionalLight directionalLight, PointLight pointLight) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    bind();
    glUniformMatrix4fv(0, 1, false, &getModelMat()[0][0]);
    glUniformMatrix4fv(1, 1, false, &camera.getPVMat()[0][0]);
    glUniform3fv(2, 1, &directionalLight.getDir()[0]);
    glUniform3fv(3, 1, &directionalLight.getLightColor()[0]);
    glUniform3fv(4, 1, &camera.pos[0]);
    glUniform3fv(5, 1, &pointLight.pos[0]);
    glUniform3fv(6, 1, &pointLight.getLightColor()[0]);
    glDrawArrays(GL_TRIANGLES, 0, (int)vertices.size());
}

void Cube::setBuffers() {
    bindVertexArray();
    // vboId already bound
    glBufferData(GL_ARRAY_BUFFER, (int)(vertices.size() * sizeof(glm::vec3)), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, normVBO);
    glBufferData(GL_ARRAY_BUFFER, (int)(normals.size() * sizeof(glm::vec3)), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(glm::vec3), nullptr);
}

void Cube::setShaders() {
    compileShadersFromFile("cubeVS.glsl", "cubeFS.glsl");
}

Cube::~Cube() {
    glDeleteBuffers(1, &normVBO);
}

