#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout(location = 0) uniform mat4 vp; // view and projection matrix
layout(location = 1) uniform vec3 dlDirWorldspace;
layout(location = 3) uniform vec3 cameraPosWorldspace;
layout(location = 4) uniform vec3 plPosWorldspace;

layout(location = 0) in vec3 vertexPosModelspace;
layout(location = 1) in vec4 posAndScale; // x, y, z - pos; w - scale (for all three dimensions)
layout(location = 2) in vec3 bubbleDiffCol; // r, g, b

out vec3 vertexPosWorldspace;
out vec3 diffuseCol;
out vec3 normalWorldspace;
out vec3 lightDirWorldspace;
out vec3 eyeDirWorldspace;
out vec3 pointLightDirWorldspace;

void main(void) {
    float s = posAndScale.w;
    mat4 m = // translation matrix
                 mat4(vec4(1, 0, 0, 0),
                      vec4(0, 1, 0, 0),
                      vec4(0, 0, 1, 0),
                      vec4(vec3(posAndScale), 1)) *
                 // scale matrix
                 mat4(vec4(s, 0, 0, 0),
                      vec4(0, s, 0, 0),
                      vec4(0, 0, s, 0),
                      vec4(0, 0, 0, 1));
    vertexPosWorldspace = vec3(m * vec4(vertexPosModelspace, 1.f));
    gl_Position = vp * vec4(vertexPosWorldspace, 1.f);

    diffuseCol = bubbleDiffCol;
    normalWorldspace = normalize(transpose(inverse(mat3(m))) * normalize(vertexPosModelspace));
    lightDirWorldspace = normalize(-dlDirWorldspace);
    eyeDirWorldspace = normalize(cameraPosWorldspace - vertexPosWorldspace);
    pointLightDirWorldspace = normalize(plPosWorldspace - vertexPosWorldspace);
}
