#version 330
#extension GL_ARB_explicit_uniform_location : require

const float alpha = .5f;

in vec3 fragmentColor;

out vec4 color;

void main(void) {
    color = vec4(fragmentColor, alpha);
}
