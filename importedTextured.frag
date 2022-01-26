#version 330
#extension GL_ARB_explicit_uniform_location : require

uniform sampler2D diffuseSampler;

in vec2 uvFrag;

out vec4 color;

void main(void) {
    color = texture(diffuseSampler, uvFrag);
}
