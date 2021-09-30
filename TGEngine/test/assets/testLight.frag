#version 460

layout(location=0) in vec2 UV;

layout(location=0) out vec4 COLOR;
layout(location=1) out vec4 NORMAL;
layout(location=2) out float ROUGHNESS;
layout(location=3) out float METALLIC;

layout(binding=0) uniform sampler SAMP;
layout(binding=1) uniform texture2D TEX;

void main() {
	color = texture(sampler2D(tex, samp), uv);
	NORMAL = vec4(1, 1, 1, 1);
	ROUGHNESS = 0;
	METALLIC = 0;
}
