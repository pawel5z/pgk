#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "AGL3Drawable.hpp"

class Background : public AGLDrawable {
public:
    Background();
    void draw(GLfloat aspect) override;
    void draw(GLfloat aspect, GLdouble progress, GLdouble quitTime, bool quitting);

private:
    void setShaders();
};

#endif //BACKGROUND_HPP
