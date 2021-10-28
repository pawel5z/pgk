#include "Background.h"

Background::Background() : AGLDrawable(0) {
    setShaders();
}

void Background::draw(GLfloat aspect) {
    bindProgram();
    glUniform1f(0, aspect);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Background::setShaders() {
    compileShadersFromFile("backgroundVS.glsl", "backgroundFS.glsl");
}
