#include "Sphere.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>

void Sphere::draw(Camera camera) {
    bind();
    glm::mat4 mvp = camera.getPVMat() * getModelMat();
    glUniformMatrix4fv(0, 1, false, &mvp[0][0]);
    glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_SHORT, nullptr);
}

void Sphere::setBuffers() {
    bindVertexArray();
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vertices.size() * sizeof(glm::vec3)), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), nullptr);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)(indices.size() * sizeof(GLushort)), indices.data(), GL_STATIC_DRAW);
}

void Sphere::setShaders() {
    compileShadersFromFile("sphereVS.glsl", "sphereFS.glsl");
}

Sphere::Sphere(int n, bool polar) : AGLDrawable() {
    if (polar)
        initWithPolarCoords(n);
    else
        initWithTetrahedron(n);
    setBuffers();
    setShaders();
}

void Sphere::initWithPolarCoords(int n) {
    if (n < 3) {
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
    auto hash = [](const glm::vec3 &v) { return std::hash<float>{}(v.x) * std::hash<float>{}(v.y) * std::hash<float>{}(v.z); };
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
}

void Sphere::initWithTetrahedron(int n) {
    if (n < 0) {
        fprintf(stderr, "incorrect sphere smoothness parameter: %d\nresetting to default: 9\n", n);
        n = 9;
    }
    vertices = {
            {0, 0, 0}, {1, 1, 0}, {0, 1, 1},
            {0, 0, 0}, {1, 0, 1}, {1, 1, 0},
            {0, 0, 0}, {1, 0, 1}, {0, 1, 1},
            {1, 0, 1}, {1, 1, 0}, {0, 1, 1}
    };
    // move tetrahedron so that its center is [0, 0, 0]
    for (auto &vertex : vertices)
        vertex = vertex - glm::vec3(0.5f, 0.5f, 0.5f);
    // smoothen the sphere
    for (size_t i = 0; i < n; i++) {
        size_t newBegin = vertices.size();
        // for every existing wall replace it with 3 new ones
        for (size_t wallInd = 0; wallInd < newBegin; wallInd += 3) {
            glm::vec3 newVertex = (vertices[wallInd] + vertices[wallInd + 1] + vertices[wallInd + 2]) / 3.0f;
            for (int j = 0, k = 0, l = 1; j < 3; j++, k = (k + 1) % 3, l = (l + 1) % 3) {
                vertices.push_back(vertices[wallInd + k]);
                vertices.push_back(vertices[wallInd + l]);
                vertices.push_back(newVertex);
            }
        }
        vertices.erase(vertices.begin(), vertices.begin() + (int)newBegin);
    }
    for (auto &vertex : vertices)
        vertex = glm::normalize(vertex);
// TODO Store only unique vertices. Make use of element buffer.
}