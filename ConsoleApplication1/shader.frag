#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0)out vec4 color;

layout(constant_id = 1) const int arraySize = 12;
in vec3 colors[arraySize];
flat in int vert;

void main(){
   color = vec4(colors[vert],1.0);
}