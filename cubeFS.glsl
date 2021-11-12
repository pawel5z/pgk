#version 330
#extension GL_ARB_explicit_uniform_location : require

in vec3 fragmentColor;

out vec4 color;

void main(void) {
    color = vec4(fragmentColor, 1.0);
}
