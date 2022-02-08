#version 460

layout(location=0) out vec4 vFragColor;
layout(location=1) in  vec4 vcolor;

layout(std140, binding = 0) uniform Modifier {
    float strength;
    vec3 color;
};

void main() {
    vFragColor.a = vcolor.a;
    vFragColor.rgb = strength*color*vcolor.rgb;
}