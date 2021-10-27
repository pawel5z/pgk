#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout(location = 0) uniform vec2  objCenter;
layout(location = 1) uniform float scale;
layout(location = 2) uniform float rot;
layout(location = 3) uniform vec4[3] vcolors;

layout(location = 0) in vec2 pos;

out vec4 vcolor;

void rotate(in vec2 pos, out vec2 rotPos) {
    mat2x2 rotMat = mat2x2(
        cos(rot), sin(rot),
        -sin(rot), cos(rot)
    );
    rotPos = rotMat * pos;
}

void main(void) {
    vcolor = vcolors[gl_VertexID];
    vec2 rotPos;
    rotate(pos, rotPos);
    gl_Position = vec4(rotPos * scale + objCenter, 0.0, 1.0);
}
