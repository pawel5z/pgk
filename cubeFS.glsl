#version 330
#extension GL_ARB_explicit_uniform_location : require

const vec4 color = vec4(0.094f, 0.623f, 0.949f, 1.f);

out vec4 fragColor;

void main(void) {
    fragColor = color;
}
