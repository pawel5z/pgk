#include "utils.hpp"

#include <stdio.h>

int fastPowAcc(int a, int n, int acc) {
    if (n == 1)
        return acc;
    if (n % 2 == 0)
        return fastPowAcc(a, n / 2, acc * acc);
    else
        return fastPowAcc(a, n / 2, acc * acc * a);
}

int fastPow(int a, int n) {
    if (n == 0)
        return 1;
    return fastPowAcc(a, n, a);
}

void dumpVertexArrayInfo() {
    GLint vertexArrayId;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vertexArrayId);
    fprintf(stderr, ">>> Vertex Array %i\n", vertexArrayId);
    GLint id;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &id);
    fprintf(stderr, "Currently bound array buffer id: %i\n", id);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &id);
    fprintf(stderr, "Currently bound element buffer id: %i\n", id);
    fprintf(stderr, "<<< Vertex Array %i\n", vertexArrayId);
}
