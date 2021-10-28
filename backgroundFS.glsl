#version 330
#extension GL_ARB_explicit_uniform_location : require

in vec4 vcolor;
in vec2 pos;

out vec4 color;

void main(void) {
    color = vcolor * abs(sin(32.0f * (pos.x * pos.y)));
}
