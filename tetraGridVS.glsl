#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout(location = 0) uniform mat4 vp;

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in mat4 model;
layout(location = 2) in vec3 pos;

out vec4 fragmentColor;

void main(void) {
    gl_Position = vp * model * vec4(vertexPos, 1);
    fragmentColor = vec4(pos, 1);
}
