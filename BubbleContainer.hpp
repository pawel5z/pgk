#ifndef BUBBLECONTAINER_HPP
#define BUBBLECONTAINER_HPP

#include "AGL3Drawable.hpp"
#include "Bubble.hpp"
#include "DirectionalLight.hpp"
#include "Sphere.hpp"

class BubbleContainer : AGLDrawable {
public:
    explicit BubbleContainer(int smoothness = 20, int maxBubbles = 1000, int spawnFreq = 10, float floatingSpd = .5f, float yMin = 0.f, float yMax = 0.f, float xMin = 0.f, float xMax = 0.f, float zMin = 0.f, float zMax = 0.f, float initScale = 1.f, float targetScale = 1.3f);
    ~BubbleContainer();
    void draw(Camera camera) override;
    void draw(Camera camera, DirectionalLight directionalLight);
    void update(float deltaTime);
    bool doesCollide(const Sphere &s);

protected:
    void setBuffers();
    void setShaders();

private:
    GLuint posAndScaleVBO{}, colVBO{}; // to clean in destructor
    std::vector<glm::vec4> posAndScaleData;
    std::vector<glm::vec3> colorData;
    // mesh data for sphere modelling a bubble
    std::vector<glm::vec3> vertices;
    std::vector<GLushort> indices;
    int spawnFreq; // bubbles per second
    float floatingSpd;
    std::vector<Bubble> bubbles;
    float timeTowardsSecond = 0.f;
    int lastUsedBubbleIdx = 0;
    float yMin, yMax, xMin, xMax, zMin, zMax;
    float initScale, targetScale;

    void initWithPolarCoords(int n);
    int getUnusedBubble();
};

#endif //BUBBLECONTAINER_HPP
