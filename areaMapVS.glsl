#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

const int resolution = 1201;

layout(location = 0) uniform mat4 mvp;
layout(location = 1) uniform vec2 lowerLeft;

layout(location = 0) in float inHeight;

out float height;

void main(void) {
    float leftLo = lowerLeft.x;
    float highLa = lowerLeft.y + 1.f;
    float row = gl_VertexID / resolution;
    float col = gl_VertexID % resolution;
    vec4 vertexPosModelspace = vec4(leftLo + col / (resolution - 1.f), highLa - row / (resolution - 1.f), 0.f, 1.f);
    gl_Position = mvp * vertexPosModelspace;
    height = inHeight;
}
