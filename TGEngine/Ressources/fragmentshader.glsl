#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 colorIn;
layout(location = 1) in vec2 uv;

layout(binding = 1) uniform sampler2D image_sampler;

layout(location = 0) out vec4 colorOut;

void main(){
   colorOut = texture(image_sampler, uv);
}