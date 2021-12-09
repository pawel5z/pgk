#version 330
#extension GL_ARB_explicit_uniform_location : require

layout(location = 3) uniform vec3 dlColor;
layout(location = 4) uniform vec3 cameraPosWorldspace;
layout(location = 5) uniform vec3 plPosWorldspace;
layout(location = 6) uniform vec3 plColor;

in vec3 vertexPosWorldspace;
in vec3 normalWorldspace;
in vec3 lightDirWorldspace;
in vec3 eyeDirWorldspace;
in vec3 pointLightDirWorldspace;

out vec4 color;

const vec3 diffuseCol = vec3(1.f, 1.f, 1.f);
const vec3 ambientCol = vec3(1.f) * .2f;

void main(void) {
    vec3 n = normalize(normalWorldspace);
    vec3 l = normalize(lightDirWorldspace);
    vec3 e = normalize(eyeDirWorldspace);
    vec3 r = reflect(-l, n);

    vec3 lPointLight = normalize(pointLightDirWorldspace);
    vec3 rPointLight = reflect(-lPointLight, n);
    float plDist = distance(vertexPosWorldspace, plPosWorldspace);

    vec3 fragmentColor = diffuseCol * dlColor * clamp(dot(n, l), 0.f, 1.f) // directional light diffuse component
                       + vec3(1.f) * .5f * pow(clamp(dot(e, r), 0.f, 1.f), 16.f) * float(dot(l, n) > 0.f) // directional light specular component
                       + diffuseCol * plColor * clamp(dot(n, lPointLight), 0.f, 1.f) * float(dot(lPointLight, n) > 0.f) / (plDist * plDist) // point light diffuse component
                       + vec3(1.f) * .5f * pow(clamp(dot(e, rPointLight), 0.f, 1.f), 16.f) / (plDist * plDist) // point light specular component
                       + ambientCol;
    float dist = length(cameraPosWorldspace - vertexPosWorldspace);
    float c = exp(-dist * .05f);
    fragmentColor *= vec3(c, c, 1.f);
    color = vec4(fragmentColor, 1.f);
}
