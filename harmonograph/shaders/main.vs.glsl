#version 430

#define NUM_PENDULA 16

#define TAU 6.283185

struct Pendulum {
    mat2 basis;
    float phase;
    float period;
    float decay;
};

layout(location=0) uniform vec2 bounds;
layout(location=1) uniform float offset;
layout(location=2) uniform int count;
layout(location=3) uniform int used_pendula;

layout(std140, binding=1) uniform Pendula {
    Pendulum pendula[NUM_PENDULA];
};

vec2 cis(float theta) {
    return vec2(cos(theta), sin(theta));
}

void main() {
    float a = float(gl_VertexID) / float(count - 1);
    float t = mix(bounds.x, bounds.y, a) + offset;

    vec2 p = vec2(0);

    for (int i = 0; i < used_pendula; i++) {
        Pendulum pen = pendula[i];

        float theta = t * TAU / pen.period;
        float scale = exp(-t * pen.decay);

        p += pen.basis * cis(theta + pen.phase) * scale;
    }

    gl_Position = vec4(p, 0, 1);
}