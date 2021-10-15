#include "CirclePolygon.hpp"

#include <glm/glm.hpp>
#include <cmath>
#include <exception>
#include <cstdio>

CirclePolygon::CirclePolygon(int n, GLfloat radius) : AGLDrawable(0), radius(radius) {
    if (n <= 2)
        throw std::invalid_argument("circle must be approximated by at least 3 vertices");
    this->n = n;
    setShaders();
    setBuffers();
}

void CirclePolygon::setShaders() {
    compileShaders(R"END(

         #version 330
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 0) in vec2 pos;
         layout(location = 0) uniform float scale;
         layout(location = 1) uniform vec2  center;
         out vec4 vtex;

         void main(void) {
            vec2 p = (pos * scale + center);
            gl_Position = vec4(p, 0.0, 1.0);
         }

      )END", R"END(

         #version 330
         #extension GL_ARB_explicit_uniform_location : require
         layout(location = 3) uniform vec3  circleColor;
         out vec4 color;

         void main(void) {
            color = vec4(circleColor,1.0);
         }

      )END");
}

void CirclePolygon::setBuffers() {
    bindBuffers();
    GLfloat vert[n][2]; // line loop vertices
    for (int i = 0; i < n; i++) {
        GLfloat t = 2 * M_PI * static_cast<GLfloat>(i) / static_cast<GLfloat>(n);
        vert[i][0] = glm::cos(t);
        vert[i][1] = glm::sin(t);
    }

    glBufferData(GL_ARRAY_BUFFER, n * 2 * sizeof(float), vert, GL_STATIC_DRAW);
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

void CirclePolygon::draw() {
    bindProgram();
    bindBuffers();
    glUniform1f(0, radius);  // scale  in vertex shader
    glUniform2f(1, x, y);  // center in vertex shader
    glUniform3f(3, circleColor[0], circleColor[1], circleColor[2]);

    glDrawArrays(GL_LINE_LOOP, 0, n);
}

void CirclePolygon::setColor(float r, float g, float b){
    circleColor[0]=r;circleColor[1]=g;circleColor[2]=b;
}
