#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout(location = 0) uniform vec2  objCenter;
layout(location = 1) uniform float scale;
layout(location = 2) uniform float rot;
layout(location = 3) uniform float aspect;
layout(location = 4) uniform vec4[3] vcolors;

layout(location = 0) in vec2 pos;

out vec4 vcolor;

void rotate(in vec2 pos, out vec2 rotPos) {
    mat2x2 rotMat = mat2x2(
        cos(rot), sin(rot),
        -sin(rot), cos(rot)
    );
    rotPos = rotMat * pos;
}

void compensateForAspect(inout vec2 v) {
    v = vec2(float(aspect > 1.0f) * v.x / aspect + float(aspect <= 1.0f) * v.x,
             float(aspect < 1.0f) * v.y * aspect + float(aspect >= 1.0f) * v.y);
}

void main(void) {
    vcolor = vcolors[gl_VertexID];
    vec2 rotPos;
    rotate(pos, rotPos);
    vec2 position = rotPos * scale + objCenter;
    compensateForAspect(position);
    gl_Position = vec4(position, 0.0, 1.0);
}
