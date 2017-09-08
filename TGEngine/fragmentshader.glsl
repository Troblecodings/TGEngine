#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0)in vec4 incolor;
layout(location = 0)out vec4 outcolor;

void main(){
   outcolor = incolor;
}