#include "BubbleContainer.hpp"

#include <unordered_map>
#include <algorithm>

BubbleContainer::BubbleContainer(int smoothness, int maxBubbles, int spawnFreq, float floatingSpd, float yMin, float yMax, float xMin, float xMax, float zMin, float zMax, float initScale, float targetScale) : spawnFreq(spawnFreq), floatingSpd(floatingSpd), yMin(yMin), yMax(yMax), xMin(xMin), xMax(xMax), zMin(zMin), zMax(zMax), initScale(initScale), targetScale(targetScale) {
    initWithPolarCoords(smoothness);
    glGenBuffers(1, &posAndScaleVBO);
    glGenBuffers(1, &colVBO);
    setBuffers();
    setShaders();
    for (int i = 0; i < maxBubbles; i++)
        bubbles.emplace_back(initScale);
    posAndScaleData = std::vector<glm::vec4>(maxBubbles);
    colorData = std::vector<glm::vec3>(maxBubbles);
}

BubbleContainer::~BubbleContainer() {
    glDeleteBuffers(1, &posAndScaleVBO);
    glDeleteBuffers(1, &colVBO);
}

void BubbleContainer::draw(Camera camera) {
    bind();
    glUniformMatrix4fv(0, 1, false, &camera.getPVMat()[0][0]);

    for (auto &bubble : bubbles)
        if (bubble.alive)
            bubble.distToCam = glm::distance2(bubble.pos, camera.pos);
        else
            bubble.distToCam = -1.f;
    std::sort(bubbles.begin(), bubbles.end());
    size_t liveBubblesCnt = 0;
    for (auto &bubble : bubbles) {
        if (bubble.alive) {
            posAndScaleData[liveBubblesCnt] = glm::vec4(bubble.pos, bubble.scale);
            colorData[liveBubblesCnt] = bubble.col;
            liveBubblesCnt++;
        }
    }

    // pass positions and scales
    glBindBuffer(GL_ARRAY_BUFFER, posAndScaleVBO);
    glBufferData(GL_ARRAY_BUFFER, (int)(liveBubblesCnt * sizeof(glm::vec4)), posAndScaleData.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, nullptr);
    glVertexAttribDivisor(1, 1);

    // pass colors
    glBindBuffer(GL_ARRAY_BUFFER, colVBO);
    glBufferData(GL_ARRAY_BUFFER, (int)(liveBubblesCnt * sizeof(glm::vec4)), colorData.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, nullptr);
    glVertexAttribDivisor(2, 1);

    glDrawElementsInstanced(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_SHORT, nullptr, (int)liveBubblesCnt);
}

void BubbleContainer::initWithPolarCoords(int n) {
    if (n < 3 || 45 < n) {
        fprintf(stderr, "incorrect bubble smoothness parameter: %d\nresetting to default: 20\n", n);
        n = 20;
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

void BubbleContainer::setBuffers() {
    bindVertexArray();
    // vboId already bound
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vertices.size() * sizeof(glm::vec3)), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), nullptr);

    // eboId already bound
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)(indices.size() * sizeof(GLushort)), indices.data(), GL_STATIC_DRAW);
}

void BubbleContainer::setShaders() {
    compileShadersFromFile("bubbleContainerVS.glsl", "bubbleContainerFS.glsl");
}

void BubbleContainer::update(float deltaTime) {
    // simulate bubbles
    for (auto &b : bubbles) {
        if (!b.alive)
            continue;
        b.pos.y += floatingSpd;
        if (b.pos.y > yMax) {
            b.alive = false;
            b.distToCam = -1.f;
            continue;
        }
        b.scale += (targetScale - b.scale) * .25f;
    }
    timeTowardsSecond += deltaTime;
    if (timeTowardsSecond >= 1.f) {
        // spawn new bubbles
        timeTowardsSecond = 0.f;
        for (int i = 0; i < spawnFreq; i++) {
            int idx = getUnusedBubble();
            bubbles[idx].alive = true;
            lastUsedBubbleIdx = idx;
            bubbles[idx].pos = glm::vec3(xMin +(xMax - xMin) * (float)rand() / (float)RAND_MAX, yMin, zMin +(zMax - zMin) * (float)rand() / (float)RAND_MAX);
            bubbles[idx].scale = 1.f;
        }
    }
    std::sort(bubbles.begin(), bubbles.end());
}

int BubbleContainer::getUnusedBubble() {
    for (int i = lastUsedBubbleIdx; i < bubbles.size(); i++)
        if (!bubbles[i].alive)
            return i;
    for (int i = 0; i < lastUsedBubbleIdx; i++)
        if (!bubbles[i].alive)
            return i;
    return 0;
}
