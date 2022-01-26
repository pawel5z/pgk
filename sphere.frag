#version 330
#extension GL_ARB_explicit_uniform_location : require

in vec3 vertexColor;

out vec4 color;

void main(void) {
    color = vec4(vertexColor, 1.f);
}