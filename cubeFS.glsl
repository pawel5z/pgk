#version 330
#extension GL_ARB_explicit_uniform_location : require

in float time;
in vec3 fragPos;

out vec4 color;

const float n = 8.0f;
const vec4 dark = vec4(0, 0, 0, 1);
const vec4 bright = vec4(.6, .6, .6, 1);
const float timeScale = 0.025;

void main(void) {
    vec3 aux = fragPos;
    aux *= aux;
    aux = aux - vec3(time) * timeScale;
    aux = floor(aux * n);
    aux.x *= float(!((abs(1. - fragPos.x) < 0.001)));
    aux.y *= float(!((abs(1. - fragPos.y) < 0.001)));
    aux.z *= float(!((abs(1. - fragPos.z) < 0.001)));
    bool isBright = mod(aux.x + aux.y + aux.z, 2.0f) == 1;
    color = float(isBright) * bright + float(!isBright) * dark;
}
