#include "Sphere.hpp"

#include "utils.hpp"
#include "sharedState.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>

void Sphere::draw(Camera camera) {
    bind();
    glUniformMatrix4fv(0, 1, false, &(camera.getPVMat() * getModelMat())[0][0]);
    glDrawElements(GL_TRIANGLE_STRIP, (int)indicesCnt, GL_UNSIGNED_INT, nullptr);
}

void Sphere::setBuffers(std::vector<glm::vec3> &vertices, std::vector<GLuint> &indices) {
    bindVertexArray();
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vertices.size() * sizeof(glm::vec3)), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), nullptr);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)(indices.size() * sizeof(GLuint)), indices.data(), GL_STATIC_DRAW);
}

void Sphere::setShaders() {
    compileShadersFromFile("sphere.vert", "sphere.frag");
}

Sphere::Sphere(int stacks, int sectors) : AGLDrawable(), stacks(stacks), sectors(sectors) {
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;

    initWithPolarCoords(stacks, sectors, vertices, indices);
    indicesCnt = indices.size();
    setBuffers(vertices, indices);
    setShaders();
}

float Sphere::latitudeAng(int i, int stacks) {
    return -90.f + 180.f * (float)i / (float)stacks;
}

float Sphere::longitudeAng(int i, int sectors) {
    return (float)i / (float)sectors * 360.f;
}

void Sphere::initWithPolarCoords(int stacks, int sectors, std::vector<glm::vec3> &vertices, std::vector<GLuint> &indices) {
    if (stacks < 3 || 180 < stacks)
        throw std::invalid_argument("Incorrect stacks number: " + std::to_string(stacks) + "\n");
    if (sectors < 3 || 360 < sectors)
        throw std::invalid_argument("Incorrect sectors number: " + std::to_string(sectors) + "\n");
/* lo         lo+1
 *
 * v1 ------- v3   la+1
 *  |         |
 *  |         |
 * v0 ------- v2   la
 */
    auto hash = [](const glm::vec3 &v) { return std::hash<size_t>{}(std::hash<float>{}(v.x) * std::hash<float>{}(v.y) * std::hash<float>{}(v.z)); };
    auto comp = [](const glm::vec3 &v, const glm::vec3 &u) { return v.x == u.x & v.y == u.y && v.z == u.z; };
    std::unordered_map<glm::vec3, GLushort, decltype(hash), decltype(comp)> existing(10, hash, comp);
    for (int la = 0; la < stacks; la++) {
        for (int lo = 0; lo < sectors; lo++) {
            std::vector<glm::vec3> v {
                    pointOnSphere(latitudeAng(la, stacks), longitudeAng(lo, sectors), .5f),
                    pointOnSphere(latitudeAng(la + 1, stacks), longitudeAng(lo, sectors), .5f),
                    pointOnSphere(latitudeAng(la, stacks), longitudeAng(lo + 1, sectors), .5f),
                    pointOnSphere(latitudeAng(la + 1, stacks), longitudeAng(lo + 1, sectors), .5f)
            };
            for (auto &candidate : v)
                if (!existing.count(candidate)) {
                    vertices.push_back(candidate);
                    /* Restrict vertices size.
                     * There's a possibility for primitive restart usage, and our primitive restart index is -1ul.
                     */
                    if ((GLuint)vertices.size() - 1 == (GLuint)-1)
                        throw std::logic_error("Too many vertices.");
                    existing[candidate] = vertices.size() - 1;
                }
            indices.insert(indices.end(), {existing[v[0]], existing[v[1]], existing[v[2]], existing[v[3]]});
        }
        indices.push_back(primitiveRestartIndex);
    }
}
