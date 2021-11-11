#version 330
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout(location = 0) uniform mat4 vp;

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 pos;
layout(location = 2) in vec4 model0;
layout(location = 3) in vec4 model1;
layout(location = 4) in vec4 model2;
layout(location = 5) in vec4 model3;

out vec4 fragmentColor;

void main(void) {
    mat4 model = mat4(model0, model1, model2, model3);
    vec4 vertexWorldPos = model * vec4(vertexPos, 1);
    gl_Position = vp * vertexWorldPos;
    fragmentColor = vertexWorldPos;
}
