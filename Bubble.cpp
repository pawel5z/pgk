#include "Bubble.hpp"

#include <glm/gtx/color_space.hpp>

Bubble::Bubble(float scale) : scale(scale) {
    float hue = (float)rand() / (float)RAND_MAX;
    col = glm::rgbColor(glm::vec3(hue, 1.f, 1.f));
}

bool Bubble::operator<(const Bubble &rhs) const {
    return rhs.distToCam < distToCam;
}

bool Bubble::operator>(const Bubble &rhs) const {
    return rhs < *this;
}

bool Bubble::operator<=(const Bubble &rhs) const {
    return !(rhs < *this);
}

bool Bubble::operator>=(const Bubble &rhs) const {
    return !(*this < rhs);
}
