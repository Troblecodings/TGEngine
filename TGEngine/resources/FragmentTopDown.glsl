#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D images[4096];

struct Material {
uint diffuseTexture;
vec4 color;
vec2 uvmanipulator;
uint align;
};

layout(binding = 2) uniform MATERIALS{
    Material mat[4096];
} data;

layout(location = 0) in vec2 uv;
layout(location = 1) flat in uint id;

layout(location = 0) out vec4 colorOut;

void main() {
    Material material = data.mat[id];
    colorOut = texture(images[material.diffuseTexture], uv + material.uvmanipulator) * material.color;
}
