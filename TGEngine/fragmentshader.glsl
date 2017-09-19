#version 460
#pragma debug(on)
#pragma optimize(off)
#extension GL_ARB_separate_shader_objects : enable

layout(location = 1)in vec4 incolor;
layout(location = 0)out vec4 fragColor;

void main(){
   fragColor = vec4(incolor);
}