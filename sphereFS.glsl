#version 330
#extension GL_ARB_explicit_uniform_location : require

layout(location = 3) uniform vec3 dlColor;
layout(location = 4) uniform vec3 cameraPosWorldspace;

in vec3 vertexPosWorldspace;
in vec3 diffuseCol;
in vec3 normalWorldspace;
in vec3 lightDirWorldspace;
in vec3 eyeDirWorldspace;

out vec4 color;

const vec3 ambientCol = vec3(1.f) * .1f;

void main(void) {
    vec3 n = normalize(normalWorldspace);
    vec3 l = normalize(lightDirWorldspace);
    vec3 e = normalize(eyeDirWorldspace);
    vec3 r = reflect(-l, n);
    vec3 fragmentColor = diffuseCol * dlColor * clamp(dot(n, l), 0.f, 1.f); // diffuse component
                       + vec3(.5f, .5f, .5f) * pow(clamp(dot(e, r), 0.f, 1.f), 7.f) * float(dot(l, n) > 0.f) // specular component
                       + ambientCol;
    float dist = length(cameraPosWorldspace - vertexPosWorldspace);
    float c = exp(-dist * .05f);
    fragmentColor *= vec3(c, c, 1.f);
    color = vec4(fragmentColor, 1.0);
}
