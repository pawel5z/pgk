#ifndef AREAFRAGMENT_HPP
#define AREAFRAGMENT_HPP

#include "AGL3Drawable.hpp"

#include <string>
#include <vector>
#include <epoxy/gl.h>

class AreaFragment {
public:
    explicit AreaFragment(std::string filePath);
    AreaFragment(const AreaFragment &o);
    AreaFragment& operator=(const AreaFragment &o);
    ~AreaFragment();
    GLshort getLeftLo() const;
    GLshort getRightLo() const;
    GLshort getLowLa() const;
    GLshort getHighLa() const;
    void prepareForDrawing();

private:
    GLuint vbo{}; // delete in destructor
    GLshort leftLo, lowLa;

    void bindVbo() const;
    GLuint getVbo() const;
};

#endif //AREAFRAGMENT_HPP
