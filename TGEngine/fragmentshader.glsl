#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 colorIn;
//layout(location = 1) in vec2 uv;

layout(location = 0) out vec4 colorOut;

void main(){
   colorOut = vec4(colorIn, 1);
}