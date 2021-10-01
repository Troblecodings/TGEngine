#version 450
#extension GL_KHR_vulkan_glsl: enable

layout(binding=0) uniform sampler samp;
layout(binding=1) uniform texture2D tex;

layout(location=0) in vec2 uv;

layout(location=0) out vec4 COLOR;
layout(location=1) out vec4 NORMAL;
layout(location=2) out float ROUGHNESS;
layout(location=3) out float METALLIC;

void main() {
	COLOR = texture(sampler2D(tex, samp), uv);
	NORMAL = vec4(1, 1, 1, 1);
	ROUGHNESS = 0;
	METALLIC = 0;
}
