#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout(location = 0) uniform mat4 vp; // view and projection matrix
layout(location = 1) uniform vec3 dlDirWorldspace;
layout(location = 2) uniform vec3 dlColor;
layout(location = 3) uniform vec3 cameraPosWorldspace;

layout(location = 0) in vec3 vertexPosModelspace;
layout(location = 1) in vec4 posAndScale; // x, y, z - pos; w - scale (for all three dimensions)
layout(location = 2) in vec3 diffuseCol; // r, g, b

out vec3 fragmentColor;

const vec3 ambientCol = vec3(1.f) * .1f;

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
    vec3 vertexPosWorldspace = vec3(m * vec4(vertexPosModelspace, 1.f));
    gl_Position = vp * vec4(vertexPosWorldspace, 1.f);

    vec3 n = normalize(transpose(inverse(mat3(m))) * normalize(vertexPosModelspace));
    vec3 l = normalize(-dlDirWorldspace);
    vec3 e = normalize(cameraPosWorldspace - vertexPosWorldspace);
    vec3 r = reflect(-l, n);
    fragmentColor = diffuseCol * dlColor * clamp(dot(n, l), 0.f, 1.f) // diffuse component
                  + vec3(.5f, .5f, .5f) * pow(clamp(dot(e, r), 0.f, 1.f), 20.f) * float(dot(l, n) > 0.f) // specular component
                  + ambientCol
    ;
}
