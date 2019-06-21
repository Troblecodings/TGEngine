#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_KHR_vulkan_glsl : enable

layout(binding = 0) uniform sampler2D image_sampler;

layout(location = 0) in vec4 color;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec4 colorOut;

void main(){
    colorOut = texture(image_sampler, uv) * color;
}