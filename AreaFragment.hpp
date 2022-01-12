#ifndef AREAFRAGMENT_HPP
#define AREAFRAGMENT_HPP

#include "AGL3Drawable.hpp"

#include <string>
#include <vector>
#include <epoxy/gl.h>

struct VertexData {
    GLshort x;
    GLshort y;
    GLshort h;
};

class AreaFragment : AGLDrawable {
public:
    explicit AreaFragment(std::string filePath, GLuint eboId);
    GLshort getLeftLo() const;
    GLshort getRightLo() const;
    GLshort getLowLa() const;
    GLshort getHighLa() const;
    void draw(Camera camera) override;
    void draw(GLuint elementsCnt, GLuint firstElementIdx);

private:
    GLshort leftLo, lowLa;
    std::vector<VertexData> vData;
};

#endif //AREAFRAGMENT_HPP
