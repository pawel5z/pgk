// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGLDrawable
//===========================================================================
#ifndef AGL3DRAWABLE_HPP
#define AGL3DRAWABLE_HPP

#include "Camera.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <epoxy/gl.h>
#include <epoxy/glx.h>

class AGLDrawable : public Transform {
public:
    explicit AGLDrawable(GLuint _pid=0);
    ~AGLDrawable(); // Cleanup VBO,VBO,Prog
    int compileShaders(const char *vs, const char *fs, const char *gs=nullptr);
    int compileShadersFromFile(const char *vs, const char *fs, const char *gs=nullptr);
    // bind vertex array AND program (shader)
    void bind() const;
    void bindVertexArray() const;
    void bindProgram() const;
    // get program (shader) id
    GLuint p() const;
    virtual void draw(Camera camera) = 0;

private:
    // VAO, VBO, Prog to clean in destructor
    GLuint vaoId{}, vboId{}, eboId{}, pId;

    int compileShaders(GLuint v, GLuint f, GLuint g=0);
    static GLint CompileLink(GLuint v, const char *which, int prog=0);
    static void getShaderSource(GLuint sId, const char * file);
};

#endif //AGL3DRAWABLE_HPP
