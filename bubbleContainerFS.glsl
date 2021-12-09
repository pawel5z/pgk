#version 330
#extension GL_ARB_explicit_uniform_location : require

layout(location = 2) uniform vec3 dlColor;
layout(location = 3) uniform vec3 cameraPosWorldspace;
layout(location = 4) uniform vec3 plPosWorldspace;
layout(location = 5) uniform vec3 plColor;

in vec3 vertexPosWorldspace;
in vec3 diffuseCol;
in vec3 normalWorldspace;
in vec3 lightDirWorldspace;
in vec3 eyeDirWorldspace;
in vec3 pointLightDirWorldspace;

out vec4 color;

const vec3 ambientCol = vec3(1.f) * .2f;

void main(void) {
    vec3 n = normalize(normalWorldspace);
    vec3 l = normalize(lightDirWorldspace);
    vec3 e = normalize(eyeDirWorldspace);
    vec3 r = reflect(-l, n);

    vec3 lPointLight = normalize(pointLightDirWorldspace);
    vec3 rPointLight = reflect(-lPointLight, n);
    float plDist = distance(vertexPosWorldspace, plPosWorldspace);

    float dist = length(cameraPosWorldspace - vertexPosWorldspace);
    float c = exp(-dist * .05f);
    vec3 blueStrength = vec3(c, c, 1.f);
    vec3 fragmentColor = diffuseCol * blueStrength * dlColor * clamp(dot(n, l), 0.f, 1.f) // directional light diffuse component
                       + vec3(1.f) * 3.f * pow(clamp(dot(e, r), 0.f, 1.f), 64.f) * float(dot(l, n) > 0.f) // directional light specular component
                       + diffuseCol * blueStrength * dlColor * clamp(dot(n, lPointLight), 0.f, 1.f) / (plDist * plDist) // point light diffuse component
                       + vec3(1.f) * 3.f * pow(clamp(dot(e, rPointLight), 0.f, 1.f), 64.f) * float(dot(lPointLight, n) > 0.f) / (plDist * plDist) // point light specular component
                       + ambientCol;
    color = vec4(fragmentColor, pow(1 - abs(dot(n, e)), 1.5f));
}
