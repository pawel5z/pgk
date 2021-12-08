#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout(location = 0) uniform mat4 mvp;

layout(location = 0) in vec3 vertexPos;

out vec3 fragmentColor;

void main(void) {
    gl_Position = mvp * vec4(vertexPos, 1);
    fragmentColor = vertexPos * 2.f + vec3(1.f);
}
