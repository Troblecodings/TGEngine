#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(constant_id = 0) const float r = 1;
layout(constant_id = 1) const float g = 1;
layout(constant_id = 2) const float b = 1;
layout(constant_id = 3) const float a = 1;

layout(binding = 0) uniform sampler2D image_sampler;

layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 colorOut;

void main(){
    colorOut = texture(image_sampler, uv) * vec4(r, g, b, a);
}