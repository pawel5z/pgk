#ifndef TETRAGRID_HPP
#define TETRAGRID_HPP

#include "AGL3Drawable.hpp"

class TetraGrid : public AGLDrawable {
public:
    void draw(Camera camera) override;
};

#endif //TETRAGRID_HPP
