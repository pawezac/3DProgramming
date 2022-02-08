#version 460

layout(location=0) in  vec4 vposition;
layout(location=1) in  vec4 vcolor;
layout(location=1) out  vec4 color;

layout(std140, binding=1) uniform Transformations {
    mat4 PVM;
};

void main() {
    gl_Position =  PVM * vposition;
    color = vcolor;
}