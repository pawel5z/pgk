#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout(location = 0) uniform mat4 m;
layout(location = 1) uniform mat4 vp;
layout(location = 2) uniform vec3 dlDirWorldspace;
layout(location = 3) uniform vec3 dlColor;

layout(location = 0) in vec3 vertexPos;

out vec3 fragmentColor;

void main(void) {
    gl_Position = vp * m * vec4(vertexPos, 1);

    vec3 n = normalize(vec3(transpose(inverse(m)) * vec4(vertexPos, 1)));
    vec3 l = normalize(-dlDirWorldspace);

    vec3 diffuseCol = vertexPos * 2.f + vec3(1.f);
    fragmentColor = diffuseCol * dlColor * clamp(dot(n, l), 0.f, 1.f);
}
