#ifndef AREAFRAGMENT_HPP
#define AREAFRAGMENT_HPP

#include <string>
#include <vector>
#include <epoxy/gl.h>

struct VertexData {
    GLshort x;
    GLshort y;
    GLshort h;
};

class AreaFragment {
public:
    explicit AreaFragment(std::string filePath);
    ~AreaFragment();
    void bindVbo() const;
    void prepareForDrawing() const;
    GLshort getLeftLo() const;
    GLshort getRightLo() const;
    GLshort getLowLa() const;
    GLshort getHighLa() const;

private:
    GLuint vbo{}; // delete in destructor
    GLshort leftLo, lowLa;
    std::vector<VertexData> vData;
};

#endif //AREAFRAGMENT_HPP
