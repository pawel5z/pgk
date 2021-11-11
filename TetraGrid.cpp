#include "TetraGrid.hpp"

glm::vec3 TetraGrid::latticeToScene(int i, int j, int k) const {
    return {
            (float)i / (float)(n - 1),
            (float)j / (float)(n - 1),
            (float)k / (float)(n - 1)
    };
}

TetraGrid::TetraGrid(GLuint n) : n(n) {
    // center tetrahedron mesh and normalize
    for (auto &vertex : vertices)
        vertex = glm::normalize(vertex - Transform::ONE * .5f);
    scale = Transform::ONE * .9f / (float)n / 2.0f;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                if ((i | j | k) == 0) // if all of them are zeros
                    continue;
                glm::vec3 pos = latticeToScene(i, j, k);
                poss.push_back(pos);
                glm::quat rot(glm::vec3(glm::mod((float)rand(), glm::two_pi<GLfloat>()), glm::mod((float)rand(), glm::two_pi<GLfloat>()), glm::mod((float)rand(), glm::two_pi<GLfloat>())));
                modelMats.push_back(glm::translate(glm::mat4(1.0f), pos) * glm::toMat4(rot) * glm::scale(scale));
            }
        }
    }
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
    glDrawElementsInstanced(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, nullptr, (int)(poss.size()));
}

void TetraGrid::setBuffers() {
    bindVertexArray();
    // vboId already bound
    glBufferData(GL_ARRAY_BUFFER, (int)(sizeof(glm::vec3) * vertices.size()), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), nullptr);

    // eboId already bound
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)(sizeof(GLuint) * indices.size()), indices.data(), GL_STATIC_DRAW);

    // model matrices
    glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
    glBufferData(GL_ARRAY_BUFFER, (int)(sizeof(glm::mat4) * modelMats.size()), modelMats.data(), GL_STATIC_DRAW);
    int index = 2;
    for (int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(index + i);
        glVertexAttribPointer(index + i, 4, GL_FLOAT, false, sizeof(glm::mat4), (void *)(i * sizeof(glm::vec4)));
    }
    for (int i = 0; i < 4; i++)
        glVertexAttribDivisor(index + i, 1);

    // position vectors
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, (int)(sizeof(glm::vec3) * poss.size()), poss.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(glm::vec3), nullptr);
    glVertexAttribDivisor(1, 1);
}

void TetraGrid::setShaders() {
    compileShadersFromFile("tetraGridVS.glsl", "tetraGridFS.glsl");
}
