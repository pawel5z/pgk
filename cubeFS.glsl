#version 330
#extension GL_ARB_explicit_uniform_location : require

in vec3 fragmentColor;

out vec4 outputFragColor;

void main(void) {
    outputFragColor = vec4(fragmentColor, 1.f);
}
