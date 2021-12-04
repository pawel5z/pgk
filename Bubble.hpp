#ifndef BUBBLE_HPP
#define BUBBLE_HPP

#include <glm/glm.hpp>

class Bubble {
public:
    float scale;
    glm::vec3 pos = {0.f, 0.f, 0.f};
    glm::vec3 col{};
    float distToCam = 0.f;
    bool alive = true;

    explicit Bubble(float scale = 1.f);
    bool operator<(const Bubble &rhs) const;
    bool operator>(const Bubble &rhs) const;
    bool operator<=(const Bubble &rhs) const;
    bool operator>=(const Bubble &rhs) const;
};

#endif //BUBBLE_HPP
