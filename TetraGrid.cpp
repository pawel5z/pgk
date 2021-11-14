#include "TetraGrid.hpp"

glm::vec3 TetraGrid::latticeToScene(int i, int j, int k) const {
    return {
            (float)i / (float)(n - 1),
            (float)j / (float)(n - 1),
            (float)k / (float)(n - 1)
    };
}

TetraGrid::TetraGrid(GLuint n) : n(n) {
    // move tetrahedron center to [0, 0, 0]
    for (auto &vertex : vertices)
        vertex -= Transform::ONE * .5f;
    scale = Transform::ONE * .75f * glm::root_two<GLfloat>() / 2.f / (float)n;
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
    glDrawElementsInstanced(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_BYTE, nullptr, (int)(poss.size()));
}

void TetraGrid::setBuffers() {
    bindVertexArray();
    // vboId already bound
    glBufferData(GL_ARRAY_BUFFER, (int)(sizeof(glm::vec3) * vertices.size()), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), nullptr);

    // eboId already bound
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)(sizeof(GLubyte) * indices.size()), indices.data(), GL_STATIC_DRAW);

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

glm::vec3 TetraGrid::getPos(int i) const{
    return poss.at(i);
}

GLuint TetraGrid::size() const {
    return poss.size();
}

std::vector<glm::vec3> TetraGrid::getVerticesPos(int i) const {
    std::vector<glm::vec3> ret;
    for (auto &vertex : vertices) {
        ret.emplace_back(modelMats[i] * glm::vec4(vertex, 1));
    }
    return ret;
}
