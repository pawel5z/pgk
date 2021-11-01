#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout(location = 0) uniform float aspect;
layout(location = 1) uniform float quitProgress;
layout(location = 2) uniform float quitTime;
layout(location = 3) uniform bool quitting;

layout(location = 0) in vec2 vpos;
layout(location = 1) in vec2 objCenter;
layout(location = 2) in float rot;
layout(location = 3) in float scale;
layout(location = 4) in float isPlayer;

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
    const vec4 playerColors[] = vec4[3](
        vec4(0.0f, 1.0f, 1.0f, 1.0f),
        vec4(0.0f, 1.0f, 1.0f, 1.0f),
        vec4(0.0f, 1.0f, 0.0f, 1.0f)
    );
    const vec4 obstacleColors[] = vec4[3](
        vec4(0.701f, 0.301f, 0.0f, 1.0f),
        vec4(0.701f, 0.301f, 0.0f, 1.0f),
        vec4(0.858f, 0.0f, 0.129f, 1.0f)
    );
    vcolor = isPlayer * playerColors[gl_VertexID] + (1.0f - isPlayer) * obstacleColors[gl_VertexID];
    vec2 rotPos;
    rotate(vpos, rotPos);
    vec2 position = rotPos * scale + objCenter;
    compensateForAspect(position);
    position -= float(quitting) * position * quitProgress / quitTime;
    gl_Position = vec4(position, 0.0, 1.0);
}
