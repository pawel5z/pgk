#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout(location = 0) uniform float aspect;
layout(location = 1) uniform float quitProgress;
layout(location = 2) uniform float quitTime;
layout(location = 3) uniform bool quitting;

out vec4 vcolor;
out vec2 pos;


void compensateForAspect(inout vec2 v) {
    v = vec2(float(aspect > 1.0f) * v.x / aspect + float(aspect <= 1.0f) * v.x,
    float(aspect < 1.0f) * v.y * aspect + float(aspect >= 1.0f) * v.y);
}

void main(void) {
    const vec2 vpositions[] = vec2[4](vec2(-0.9f, -0.9f),
                                      vec2(0.9f, -0.9f),
                                      vec2(-0.9f, 0.9f),
                                      vec2(0.9f, 0.9f));
    const vec4 vcolors[] = vec4[4](vec4(0.007f, 0.0f, 0.839f, 1.0f),
                                   vec4(0.415f, 0.411f, 0.807f, 1.0f),
                                   vec4(0.415f, 0.411f, 0.807f, 1.0f),
                                   vec4(0.937f, 0.458f, 0.023f, 1.0f));
    pos = vpositions[gl_VertexID];
    compensateForAspect(pos);
    pos -= float(quitting) * pos * quitProgress / quitTime;
    gl_Position = vec4(pos, 0.5, 1.0);
    vcolor = vcolors[gl_VertexID];
}
