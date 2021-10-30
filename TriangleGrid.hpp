#ifndef TRIANGLEGRID_HPP
#define TRIANGLEGRID_HPP

#include <memory>

#include "AGL3Drawable.hpp"

class TriangleGrid : public AGLDrawable {
public:
    TriangleGrid(int n, GLfloat scale);
    virtual ~TriangleGrid();
    void draw(GLfloat aspect) override;
    void draw(GLfloat aspect, GLdouble progress, GLdouble quitTime, bool quitting);
    glm::vec2 getTriVertexWorldCoords(int t, int v);
    GLfloat getRot(int t);
    glm::mat2 getRotMat(int t);
    void setRot(int t, float rot);
    glm::vec2 getPos(int t);
    void setPos(int t, glm::vec2 pos);

private:
    int n;
    GLfloat scale;
    glm::vec2 vertices[3] = {
            glm::vec2(-0.25f, -1.0f),
            glm::vec2(0.25f, -1.0f),
            glm::vec2(0.0f, 1.0f)
    };
    std::vector<glm::vec4> transforms;
    std::vector<GLfloat> bPlayers;
    // deallocate in destructor
    GLuint transformsVBO{}, isPlayerVBO{};

    void setShaders();
    void setBuffers();
    std::shared_ptr<GLfloat[]> getVerticesArray() const;
    /* Interpolate (i, j) from {0..n}x{0..n}
     * to [-0.9, 0.9]x[-0.9, 0.9]
     */
    glm::vec2 onBoard(int i, int j) {
        return {-0.9f + (GLfloat)i / (GLfloat)(n-1) * 1.8f,
                -0.9f + (GLfloat)j / (GLfloat)(n-1) * 1.8f};
    }
    GLfloat randAngle() {
        return (GLfloat)(rand() % 360) * glm::pi<GLfloat>() / 180.0f;
    }
    void updateTransforms();
};

#endif //TRIANGLEGRID_HPP
