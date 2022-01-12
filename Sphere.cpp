#include "Sphere.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>

void Sphere::draw(Camera camera) {
    bind();
    glUniformMatrix4fv(0, 1, false, &(camera.getPVMat() * getModelMat())[0][0]);
    glDrawElements(GL_TRIANGLES, (int)indicesCnt, GL_UNSIGNED_SHORT, nullptr);
}

void Sphere::setBuffers(std::vector<glm::vec3> &vertices, std::vector<GLushort> &indices) {
    bindVertexArray();
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vertices.size() * sizeof(glm::vec3)), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), nullptr);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)(indices.size() * sizeof(GLushort)), indices.data(), GL_STATIC_DRAW);
}

void Sphere::setShaders() {
    compileShadersFromFile("sphereVS.glsl", "sphereFS.glsl");
}

Sphere::Sphere(int n) : AGLDrawable() {
    std::vector<glm::vec3> vertices;
    std::vector<GLushort> indices;

    initWithPolarCoords(n, vertices, indices);
    indicesCnt = indices.size();
    setBuffers();
    setShaders();
}

void Sphere::initWithPolarCoords(int n, std::vector<glm::vec3> &vertices, std::vector<GLushort> &indices) {
    if (n < 3 || 45 < n) {
        fprintf(stderr, "incorrect sphere smoothness parameter: %d\nresetting to default: 45\n", n);
        n = 45;
    }
/*
 * lo         lo+1
 *
 * v3 ------- v2    la+1
 *  |         |
 *  |         |
 * v0 ------- v1    la
 * */
    auto hash = [](const glm::vec3 &v) { return std::hash<size_t>{}(std::hash<float>{}(v.x) * std::hash<float>{}(v.y) * std::hash<float>{}(v.z)); };
    auto comp = [](const glm::vec3 &v, const glm::vec3 &u) { return v.x == u.x & v.y == u.y && v.z == u.z; };
    std::unordered_map<glm::vec3, GLushort, decltype(hash), decltype(comp)> existing(10, hash, comp);
    for (int la = 0; la < n; la++) {
        for (int lo = 0; lo < n; lo++) {
            std::vector<glm::vec3> v {
                glm::vec3 (glm::sin((float)la / (float)n * glm::pi<GLfloat>()) * glm::cos((float)lo / (float)n * glm::two_pi<GLfloat>()),
                           glm::sin((float)la / (float)n * glm::pi<GLfloat>()) * glm::sin((float)lo / (float)n * glm::two_pi<GLfloat>()),
                           glm::cos((float)la / (float)n * glm::pi<GLfloat>())),
                glm::vec3 (glm::sin((float)la / (float)n * glm::pi<GLfloat>()) * glm::cos((float)(lo + 1) / (float)n * glm::two_pi<GLfloat>()),
                           glm::sin((float)la / (float)n * glm::pi<GLfloat>()) * glm::sin((float)(lo + 1) / (float)n * glm::two_pi<GLfloat>()),
                           glm::cos((float)la / (float)n * glm::pi<GLfloat>())),
                glm::vec3 (glm::sin((float)(la + 1) / (float)n * glm::pi<GLfloat>()) * glm::cos((float)(lo + 1) / (float)n * glm::two_pi<GLfloat>()),
                           glm::sin((float)(la + 1) / (float)n * glm::pi<GLfloat>()) * glm::sin((float)(lo + 1) / (float)n * glm::two_pi<GLfloat>()),
                           glm::cos((float)(la + 1) / (float)n * glm::pi<GLfloat>())),
                glm::vec3 (glm::sin((float)(la + 1) / (float)n * glm::pi<GLfloat>()) * glm::cos((float)lo / (float)n * glm::two_pi<GLfloat>()),
                           glm::sin((float)(la + 1) / (float)n * glm::pi<GLfloat>()) * glm::sin((float)lo / (float)n * glm::two_pi<GLfloat>()),
                           glm::cos((float)(la + 1) / (float)n * glm::pi<GLfloat>()))
            };
            for (auto &candidate : v)
                if (!existing.count(candidate)) {
                    vertices.push_back(candidate);
                    existing[candidate] = vertices.size() - 1;
                }
            indices.insert(indices.end(), {existing[v[0]], existing[v[1]], existing[v[3]]});
            indices.insert(indices.end(), {existing[v[1]], existing[v[2]], existing[v[3]]});
        }
    }
    // make sphere radius 0.5 so that it fits in 1x1x1 cube
    for (auto &v : vertices)
        v *= .5;
}
