#version 330
#extension GL_ARB_explicit_uniform_location : require

in vec3 fragPos;

out vec4 color;

const float n = 16.0f;
const float black = .4;
const float white = .6;

void main(void) {
    vec3 aux = floor(fragPos * n);
    aux.x = float(!(abs(n - aux.x) < 0.001)) + float(abs(n - aux.x) >= 0.001) * aux.x;
    aux.y = float(!(abs(n - aux.y) < 0.001)) + float(abs(n - aux.y) >= 0.001) * aux.y;
    aux.z = float(!(abs(n - aux.z) < 0.001)) + float(abs(n - aux.z) >= 0.001) * aux.z;
    float bw = black + mod(aux.x + aux.y + aux.z, 2.0f) * (white - black);
    color = vec4(vec3(bw), 1.0f);
}
