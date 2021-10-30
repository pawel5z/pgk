#include "Background.hpp"

Background::Background() : AGLDrawable(0) {
    setShaders();
}

void Background::draw(GLfloat aspect) {
    draw(aspect, 0.0, 0.0, false);
}

void Background::setShaders() {
    compileShadersFromFile("backgroundVS.glsl", "backgroundFS.glsl");
}

void Background::draw(GLfloat aspect, GLdouble progress, GLdouble quitTime, bool quitting) {
    bindProgram();
    glUniform1f(0, aspect);
    glUniform1f(1, (GLfloat)progress);
    glUniform1f(2, (GLfloat)quitTime);
    glUniform1i(3, quitting);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
