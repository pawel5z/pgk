#ifndef TETRAGRID_HPP
#define TETRAGRID_HPP

#include "AGL3Drawable.hpp"

class TetraGrid : public AGLDrawable {
public:
    TetraGrid(GLuint n);
    void draw(Camera camera) override;
    virtual ~TetraGrid();
    glm::vec3 getPos(int i) const;
    GLuint size() const;
    std::vector<glm::vec3> getVerticesPos(int i) const;

protected:
    void setBuffers();
    void setShaders();

private:
    // to clean in destructor
    GLuint modelVBO{}, posVBO{};
    GLuint n;
    std::vector<glm::vec3> vertices = {{0, 0, 0}, {0, 1, 1}, {1, 0, 1}, {1, 1, 0}};
    const std::vector<GLuint> indices = {
            0, 3, 1,
            0, 2, 3,
            0, 2, 1,
            2, 3, 1
    };
    std::vector<glm::vec3> poss;
    std::vector<glm::mat4> modelMats;

    glm::vec3 latticeToScene(int i, int j, int k) const;
};

#endif //TETRAGRID_HPP
