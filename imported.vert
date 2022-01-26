#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout(location = 0) uniform mat4 mvp;

layout(location = 0) in vec3 vertexPosModelspace;

out vec3 vertexColor;

void main(void) {
    gl_Position = mvp * vec4(vertexPosModelspace, 1);
    vertexColor = (vertexPosModelspace + vec3(.5f)) * 2.f;
}