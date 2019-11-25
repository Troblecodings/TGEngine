#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler mainsampler;
layout(binding = 2) uniform texture2D images[2048];

layout(push_constant) uniform PUSH_CONST{
    layout(offset=4) uint diffuseTexture;
    layout(offset=8) vec4 color;
    layout(offset=24) vec2 uvmanipulator;
    layout(offset=32) uint samplerId;
} pushconst;

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 colorOut;

void main() {
    colorOut = texture(sampler2D(images[pushconst.material.diffuseTexture], mainsampler), uv + material.uvmanipulator) * material.color;
}
