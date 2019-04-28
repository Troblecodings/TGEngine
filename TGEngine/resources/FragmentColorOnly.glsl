#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_KHR_vulkan_glsl : enable

layout(constant_id = 0) const float r = 0;
layout(constant_id = 1) const float g = 0;
layout(constant_id = 2) const float b = 0;
layout(constant_id = 3) const float a = 1;

layout(location = 0) in float light_amplifier;

layout(location = 0) out vec4 colorOut;

void main(){
    colorOut = vec4(vec3(r, g, b) * light_amplifier, a);
}