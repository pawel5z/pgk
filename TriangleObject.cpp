#include "TriangleObject.hpp"
#include <glm/gtc/type_ptr.hpp>

TriangleObject::TriangleObject() : AGLDrawable(0) {
    init();
}

TriangleObject::TriangleObject(glm::vec2 pos, GLfloat rot, GLfloat scale) : AGLDrawable(0), rot(rot) {
    this->pos = pos;
    this->scale = scale;
    init();
}

void TriangleObject::draw() {
    bindProgram();
    bindBuffers();
    // TODO pass uniforms
    glUniform2fv(0, 1, glm::value_ptr(pos));
    glUniform1f(1, scale);
    glUniform1f(2, rot);
    glUniform4fv(3, 3, colors);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void TriangleObject::setShaders() {
    compileShadersFromFile("triVS.glsl", "triFS.glsl");
}

void TriangleObject::setBuffers() {
    bindBuffers();
    GLfloat vert[3][2] = {
            {-0.25f, -1.0f},
            {0.25f, -1.0f},
            {0.0f, 1.0f}
    };
    glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(GLfloat), vert, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0,                  // attribute 0, must match the layout in the shader.
            2,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,//24,             // stride
            (void*)0            // array buffer offset
    );
}

void TriangleObject::setVertexColor(int i, const glm::vec4 &color) {
    if (i < 0 || 3 <= i)
        throw std::invalid_argument("vertex index out of range: " + std::to_string(i));
    for (int j = 0; j < 4; j++)
        colors[4 * i + j] = color[j];
}

void TriangleObject::init() {
    setShaders();
    setBuffers();
    // init rotation matrix
    setRot(rot);
}

GLfloat TriangleObject::getRot() const {
    return rot;
}

void TriangleObject::setRot(GLfloat rot) {
    this->rot = rot;
    rotMat[0] = glm::vec2(glm::cos(rot), glm::sin(rot));
    rotMat[1] = glm::vec2(-glm::sin(rot), glm::cos(rot));
}

const glm::mat2 &TriangleObject::getRotMat() const {
    return rotMat;
}
