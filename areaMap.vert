#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

const int resolution = 1201;

layout(location = 0) uniform mat4 mvp;
layout(location = 1) uniform vec2 lowerLeft;
layout(location = 2) uniform float midLa;

layout(location = 0) in float inHeight;

out float height;

void main(void) {
    float leftLo = lowerLeft.x;
    float highLa = lowerLeft.y + 1.f;
    float la = highLa - float(gl_VertexID / resolution) / (resolution - 1.f);
    float lo = leftLo + float(gl_VertexID % resolution) / (resolution - 1.f);
    vec4 vertexPosModelspace = vec4(lo * cos(radians(midLa)), la, 0.f, 1.f);
    gl_Position = mvp * vertexPosModelspace;
    height = inHeight;
}
