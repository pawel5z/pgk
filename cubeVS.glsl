#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout(location = 0) uniform mat4 mvp;
layout(location = 1) uniform float inTime;

layout(location = 0) in vec3 vertexPos;

out vec3 fragPos;
out float time;

void main(void) {
    gl_Position = mvp * vec4(vertexPos, 1);
    fragPos = vertexPos * 2.0f + vec3(.5, .5, .5);
    time = inTime;
}
