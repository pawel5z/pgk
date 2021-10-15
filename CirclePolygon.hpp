#ifndef CIRCLEPOLYGON_HPP
#define CIRCLEPOLYGON_HPP

#include <epoxy/gl.h>
#include <epoxy/glx.h>

#include "AGL3Drawable.hpp"

class CirclePolygon : public AGLDrawable {
public:
    CirclePolygon(int n);
    void setShaders();
    void setBuffers();
    void draw(float tx, float ty);
    void setColor(float r, float g, float b);
private:
    // number of vertices which will approximate circle
    int n;
    GLfloat circleColor[3] = {0.101, 0.956, 0.862};
};

#endif //CIRCLEPOLYGON_HPP
