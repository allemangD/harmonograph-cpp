#version 430

layout(location=1) uniform vec4 col;

out vec4 color;

void main() {
    color = col;
}