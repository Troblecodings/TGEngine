#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler mainsampler;
layout(binding = 2) uniform texture2D images[2048];

layout(push_constant) uniform PUSH_CONST{
    layout(offset=64) vec4 color;
	uint diffuseTexture;
    uint animationIndex;
} pushconst;

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 colorOut;

void main() {
    // Put UV calculation in front to give it something to work on swap
    vec2 calculatedUV = (uv + ubblock.uvOffsets[pushconst.animationIndex]);
    colorOut = texture(sampler2D(images[pushconst.diffuseTexture], mainsampler), calculatedUV) * pushconst.color;
}
