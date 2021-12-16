#version 330
#extension GL_ARB_explicit_uniform_location : require

layout(location = 3) uniform vec3 dlColor;

in float height;

out vec4 color;

void main(void) {
    color = vec4(0.f);
    color.r = float(500.f <= height && height < 1000.f) * (height / 500.f - 1 .f) + float(1000.f <= height);
    color.g = float(0.f <= height && height < 500.f) * height / 500.f
            + float(500.f <= height && height < 1000.f)
            + float(1000.f <= height && height < 2000.f) * (2.f - height / 1000.f)
            + float(2000.f <= height) * (height / 2000.f - 1.f);
    color.b = float(height < 0.f)
            + float(2000.f <= height) * (height / 2000.f - 1.f);
    color.a = 1.f;
}
