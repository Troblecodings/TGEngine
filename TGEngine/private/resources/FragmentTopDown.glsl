#version 460

layout(binding = 1) uniform sampler mainsampler;
layout(binding = 2) uniform texture2D images[2048];

layout(push_constant) uniform PUSH_CONST{
    layout(offset=72) float r;
    float g;
    float b;
    float a;
    uint diffuseTexture;
} pushconst;

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 colorOut;

void main() {
    colorOut = texture(sampler2D(images[pushconst.diffuseTexture], mainsampler), uv) * vec4(pushconst.r, pushconst.g, pushconst.b, pushconst.a);
}
