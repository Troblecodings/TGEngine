#version 460

layout(binding=0) uniform sampler samp;
layout(binding=1) uniform texture2D tex;

layout(location=0) in vec2 uv;

layout(location=0) out vec4 color;

void main() {
	color = texture(sampler2D(tex, samp), uv);
}
