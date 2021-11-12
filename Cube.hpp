#ifndef CUBE_HPP
#define CUBE_HPP

#include "AGL3Drawable.hpp"

class Cube : public AGLDrawable {
public:
    Cube();
    void draw(Camera camera) override;
};

#endif //LAB4_CUBE_HPP
