#ifndef TRIANGLEOBJECT_HPP
#define TRIANGLEOBJECT_HPP

#include <memory>

#include "AGL3Drawable.hpp"

class TriangleObject : public AGLDrawable {
public:
    GLfloat colors[3 * 4] = {
            0.701f, 0.301f, 0.0f, 1.0f,
            0.701f, 0.301f, 0.0f, 1.0f,
            0.858f, 0.0f, 0.129f, 1.0f
    };
    glm::vec2 vertices[3] = {
            glm::vec2(-0.25f, -1.0f),
            glm::vec2(0.25f, -1.0f),
            glm::vec2(0.0f, 1.0f)
    };

    TriangleObject();
    TriangleObject(glm::vec2 pos, GLfloat rot, GLfloat scale);
    void draw(GLfloat aspect) override;
    void draw(GLfloat aspect, GLdouble progress, GLdouble quitTime, bool quitting);
    // set i-th vertex colors
    void setVertexColor(int i, const glm::vec4 &color);
    GLfloat getRot() const;
    void setRot(GLfloat rot);
    const glm::mat2 &getRotMat() const;
    std::shared_ptr<GLfloat[]> getVerticesArray() const;
    glm::vec2 getVertexWorldCoords(int i);

private:
    GLfloat rot = 0.0f;
    glm::mat2 rotMat;

    void setShaders();
    void setBuffers();
    void init();
};

#endif //TRIANGLEOBJECT_HPP
