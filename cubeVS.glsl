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

    const vec3 diffuseCol = vec3(.094f, .623f, .949f);
    vec3 n = normalize(vec3(transpose(inverse(m)) * vec4(vertexPos, 1)));
    vec3 l = normalize(-dlDirWorldspace);
    fragmentColor = diffuseCol * dlColor * clamp(dot(n, l), 0.f, 1.f);
}
