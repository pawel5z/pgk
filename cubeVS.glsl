#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout(location = 0) uniform mat4 m;
layout(location = 1) uniform mat4 vp;
layout(location = 2) uniform vec3 dlDirWorldspace;
layout(location = 4) uniform vec3 cameraPosWorldspace;

layout(location = 0) in vec3 vertexPosModelspace;

out vec3 normalWorldspace;
out vec3 lightDirWorldspace;
out vec3 eyeDirWorldspace;

void main(void) {
    vec3 vertexPosWorldspace = vec3(m * vec4(vertexPosModelspace, 1.f));
    gl_Position = vp * vec4(vertexPosWorldspace, 1.f);

    normalWorldspace = normalize(transpose(inverse(mat3(m))) * normalize(vertexPosModelspace));
    lightDirWorldspace = normalize(-dlDirWorldspace);
    eyeDirWorldspace = normalize(cameraPosWorldspace - vertexPosWorldspace);
}
