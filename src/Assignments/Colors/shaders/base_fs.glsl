#version 410

layout(location=0) out vec4 vFragColor;
in vec4 _color;

void main() {
    vFragColor = vec4(_color[0], _color[1], _color[2], _color[3]);
}
