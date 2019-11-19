#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler mainsampler;
layout(binding = 2) uniform texture2D images[2048];

struct Material {
uint diffuseTexture;
vec4 color;
vec2 uvmanipulator;
uint samplerId;
};

layout(binding = 3) uniform MATERIALS{
    Material mat[256];
} data;

layout(location = 0) in vec2 uv;
layout(location = 1) flat in uint id;

layout(location = 0) out vec4 colorOut;

void main() {
    Material material = data.mat[id];
    colorOut = texture(sampler2D(images[material.diffuseTexture], mainsampler), uv + material.uvmanipulator) * material.color;
}
