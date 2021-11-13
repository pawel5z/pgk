#version 330
#extension GL_ARB_explicit_uniform_location : require

in float time;
in vec3 fragPos;

out vec4 color;

const float n = 4.0f;
const float dark = 0;
const float bright = .6;
const float timeScale = 0.025;

void main(void) {
    vec3 aux = fragPos;
    aux *= aux;
    aux = aux - vec3(time) * timeScale;
    aux = floor(aux * n);
    aux.x *= float(!((abs(1. - fragPos.x) < 0.001)));
    aux.y *= float(!((abs(1. - fragPos.y) < 0.001)));
    aux.z *= float(!((abs(1. - fragPos.z) < 0.001)));
    float darkOrBright = dark + mod(aux.x + aux.y + aux.z, 2.0f) * (bright - dark);

    color = vec4(vec3(darkOrBright), 1.0f);
}
