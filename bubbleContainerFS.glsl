#version 330
#extension GL_ARB_explicit_uniform_location : require

layout(location = 2) uniform vec3 dlColor;

in vec3 diffuseCol;
in vec3 normalWorldspace;
in vec3 lightDirWorldspace;
in vec3 eyeDirWorldspace;

out vec4 color;

const float alpha = .2f;
const vec3 ambientCol = vec3(1.f) * .1f;

void main(void) {
    vec3 n = normalize(normalWorldspace);
    vec3 l = normalize(lightDirWorldspace);
    vec3 e = normalize(eyeDirWorldspace);
    vec3 r = reflect(-l, n);
    vec3 fragmentColor = diffuseCol * dlColor * clamp(dot(n, l), 0.f, 1.f) // diffuse component
                  + vec3(1.f, 1.f, 1.f) * pow(clamp(dot(e, r), 0.f, 1.f), 64.f) * float(dot(l, n) > 0.f) // specular component
                  + ambientCol;
    color = vec4(fragmentColor, alpha);
}
