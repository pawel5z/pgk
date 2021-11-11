#include "TetraGrid.hpp"

TetraGrid::TetraGrid(GLuint n) : n(n) {
    scale = Transform::ONE * .9f * 1.0f / (float)n / 2.0f;
    // TODO generate tetrahedrons
    glGenBuffers(1, &modelVBO);
    glGenBuffers(1, &posVBO);
    setBuffers();
    setShaders();
}

TetraGrid::~TetraGrid() {
    glDeleteBuffers(1, &modelVBO);
    glDeleteBuffers(1, &posVBO);
}

void TetraGrid::draw(Camera camera) {
    bind();
    glUniformMatrix4fv(0, 1, false, &camera.getPVMat()[0][0]);
    glDrawElementsInstanced(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, nullptr, n * n * n - 1);
}

void TetraGrid::setBuffers() {
    bindVertexArray();
    // vboId already bound
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), nullptr);

    // veoId already bound
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

    // model matrices
    glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
    glBufferData(GL_ARRAY_BUFFER, (int)(sizeof(glm::mat4) * modelMats.size()), modelMats.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4 * 3, GL_FLOAT, false, sizeof(glm::mat4), nullptr);
    glVertexAttribDivisor(1, 1);

    // position vectors
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, (int)(sizeof(glm::vec3) * poss.size()), poss.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(glm::vec3), nullptr);
    glVertexAttribDivisor(2, 1);

}

void TetraGrid::setShaders() {
    compileShadersFromFile("tetraGridVS.glsl", "tetraGridFS.glsl");
}
