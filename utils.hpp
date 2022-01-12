#ifndef UTILS_HPP
#define UTILS_HPP

#include <epoxy/gl.h>
#include <glm/glm.hpp>

const GLuint primitiveRestartIndex = (GLuint)(-1);

int fastPow(int a, int n);
void dumpVertexArrayInfo();
glm::vec3 pointOnSphere(float la, float lo, float r=1.f);
int compileProgram(GLuint &pId, const char *vs, const char *fs, const char *gs= nullptr);
int compileProgramFromFile(GLuint &pId, const char *vs, const char *fs, const char *gs= nullptr);

#endif //UTILS_HPP
