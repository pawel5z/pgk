#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout(location = 0) uniform mat4 vp; // view and projection matrix

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec4 posAndScale; // x, y, z - pos; w - scale (for all three dimensions)
layout(location = 2) in vec3 color; // r, g, b

out vec3 fragmentColor;

void main(void) {
    float s = posAndScale.w;
    mat4 model = // translation matrix
                 mat4(vec4(1, 0, 0, 0),
                      vec4(0, 1, 0, 0),
                      vec4(0, 0, 1, 0),
                      vec4(vec3(posAndScale), 1)) *
                 // scale matrix
                 mat4(vec4(s, 0, 0, 0),
                      vec4(0, s, 0, 0),
                      vec4(0, 0, s, 0),
                      vec4(0, 0, 0, 1));
    gl_Position = vp * model * vec4(vertexPos, 1);
    fragmentColor = color;
}
