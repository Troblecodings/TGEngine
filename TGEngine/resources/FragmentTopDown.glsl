#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler mainsampler;
layout(binding = 2) uniform texture2D images[2048];
layout(binding = 3) uniform UV_OFFSETTING {
    vec2 uvOffsets[2000];
} ubblock;

layout(push_constant, std140) uniform PUSH_CONST{
    layout(offset=64) vec4 color;
	uint diffuseTexture;
    uint animationIndex;
} pushconst;

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 colorOut;

void main() {
    vec2 calculatedUV = (uv + ubblock.uvOffsets[pushconst.animationIndex]);
    colorOut = texture(sampler2D(images[pushconst.diffuseTexture], mainsampler), calculatedUV) * pushconst.color;
}
