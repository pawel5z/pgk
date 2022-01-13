#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

const float resolution = 1201.f;
const float earthRadius = 6371.008f;

layout(location = 0) uniform mat4 mvp;
layout(location = 1) uniform vec2 lowerLeft;

layout(location = 0) in float inHeight;

out float height;

void main(void) {
    float leftLo = lowerLeft.x;
    float highLa = lowerLeft.y + 1.f;
    float row = float(gl_VertexID) / resolution;
    float col = mod(float(gl_VertexID), resolution);
    float lo = leftLo + col / 1200.f + 180.f;
    float la = highLa - row / 1200.f;
    vec4 vertexPosModelspace = vec4(0.f, 0.f, 0.f, 1.f);
    vertexPosModelspace.xyz = vec3(
        sin(radians(lo)) * cos(radians(la)),
        sin(radians(la)),
        cos(radians(la)) * cos(radians(lo))
    ) * (earthRadius + inHeight / 1000.f);
    gl_Position = mvp * vertexPosModelspace;
    height = inHeight;
}