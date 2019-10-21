#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D images[4096];

layout(location = 0) in vec2 uv;
layout(location = 1) flat in uint id;

layout(location = 0) out vec4 colorOut;

void main() {
    colorOut = texture(images[id], uv);
}
