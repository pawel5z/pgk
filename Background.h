#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "AGL3Drawable.hpp"

class Background : public AGLDrawable {
public:
    Background();
    void draw(GLfloat aspect) override;
    void draw(GLfloat aspect, GLdouble progress, GLdouble quitTime, bool quitting);

private:
    void setShaders();
};

#endif //BACKGROUND_H
