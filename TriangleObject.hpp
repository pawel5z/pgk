#ifndef TRIANGLEOBJECT_HPP
#define TRIANGLEOBJECT_HPP

#include "AGL3Drawable.hpp"

class TriangleObject : public AGLDrawable {
public:
    GLfloat rot = 0.0f;
    GLfloat colors[3 * 4] = {
            0.701f, 0.301f, 0.0f, 1.0f,
            0.701f, 0.301f, 0.0f, 1.0f,
            0.858f, 0.0f, 0.129f, 1.0f
    };

    TriangleObject();
    TriangleObject(glm::vec2 pos, GLfloat rot, GLfloat scale);
    void draw() override;
    // set i-th vertex colors
    void setVertexColor(int i, const glm::vec4 &color);

private:
    void setShaders();
    void setBuffers();
};


#endif //TRIANGLEOBJECT_HPP
