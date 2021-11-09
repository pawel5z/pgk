#include "TriangleGrid.hpp"
#include "AGL3Window.hpp"

#include <memory>

TriangleGrid::TriangleGrid(int n, GLfloat scale) : n(n), scale(scale) {
    transforms = std::vector<glm::vec4>(n * n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            transforms[i * n + j] = glm::vec4(onBoard(i, j), randAngle(), this->scale);
        }
    }
    bPlayers = std::vector<GLfloat>(n * n);
    bPlayers[0] = 1.0f;
    for (int i = 1; i < n * n; i++)
        bPlayers[i] = 0.0f;
    glGenBuffers(1, &transformsVBO);
    glGenBuffers(1, &isPlayerVBO);
    setShaders();
    setBuffers();
}

void TriangleGrid::draw(GLfloat aspect) {
    draw(aspect, 0.0, 1.0, false);
}

void TriangleGrid::setShaders() {
    compileShadersFromFile("triGridVS.glsl", "triGridFS.glsl");
}

void TriangleGrid::setBuffers() {
    bindBuffers();
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec2), getVerticesArray().get(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0,                  // attribute 0, must match the layout in the shader.
            2,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );

    // bind transforms
    glBindBuffer(GL_ARRAY_BUFFER, transformsVBO);
    glBufferData(GL_ARRAY_BUFFER, n * n * sizeof(glm::vec4), transforms.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)(sizeof(glm::vec2)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)(sizeof(glm::vec3)));

    // bind player booleans
    glBindBuffer(GL_ARRAY_BUFFER, isPlayerVBO);
    glBufferData(GL_ARRAY_BUFFER, n * n * sizeof(GLfloat), bPlayers.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
}

std::shared_ptr<GLfloat[]> TriangleGrid::getVerticesArray() const {
    std::shared_ptr<GLfloat[]> verticesArray(new GLfloat[6]);
    for (int i = 0; i < 3; i++) {
        verticesArray.get()[2 * i] = vertices[i].x;
        verticesArray.get()[2 * i + 1] = vertices[i].y;
    }
    return verticesArray;
}

void TriangleGrid::draw(GLfloat aspect, GLdouble progress, GLdouble quitTime, bool quitting) {
    bindProgram();
    bindBuffers();
    glUniform1f(0, aspect);
    glUniform1f(1, (GLfloat)progress);
    glUniform1f(2, (GLfloat)quitTime);
    glUniform1i(3, quitting);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, n * n);
}

TriangleGrid::~TriangleGrid() {
    glDeleteBuffers(1, &transformsVBO);
    glDeleteBuffers(1, &isPlayerVBO);
}

GLfloat TriangleGrid::getRot(int t) {
    return transforms.at(t)[2];
}

glm::mat2 TriangleGrid::getRotMat(int t) {
    GLfloat rot = getRot(t);
    glm::mat2 rotMat = glm::mat2(
            glm::vec2(glm::cos(rot), glm::sin(rot)),
            glm::vec2(-glm::sin(rot), glm::cos(rot)));
    return rotMat;
}

glm::vec2 TriangleGrid::getTriVertexWorldCoords(int t, int v) {
    if (v < 0 || 2 < v)
        throw std::invalid_argument("vertex index out of range: " + std::to_string(v));
    return getPos(t) + scale * getRotMat(t) * vertices[v];
}

glm::vec2 TriangleGrid::getPos(int t) {
    return {transforms.at(t).x, transforms.at(t).y};
}

void TriangleGrid::setRot(int t, float rot) {
    transforms.at(t)[2] = rot;
    updateTransform(t);
}

void TriangleGrid::updateTransform(int t) {
    bindBuffers();
    glBindBuffer(GL_ARRAY_BUFFER, transformsVBO);
    glBufferSubData(GL_ARRAY_BUFFER, t * sizeof(glm::vec4), sizeof(glm::vec4), transforms.data() + t);
}

void TriangleGrid::setPos(int t, glm::vec2 pos) {
    transforms.at(t).x = pos.x;
    transforms.at(t).y = pos.y;
    updateTransform(t);
}