#version 410

in vec4 v_color;

layout(std140) uniform Modifiers {
   float strength;
   vec3  light;
};

layout(location=0) out vec4 vFragColor;

void main() {
    vFragColor.a  =  1.0;
    vFragColor.rgb  =  strength*light*v_color.rgb;
}
