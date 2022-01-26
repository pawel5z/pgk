#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout(location = 0) uniform mat4 mvp;

layout(location = 0) in vec3 vertexPosModelspace;
layout(location = 1) in vec2 uv;

out vec2 uvFrag;

void main(void) {
    gl_Position = mvp * vec4(vertexPosModelspace, 1);
    uvFrag = uv;
}
