#version 460

layout(binding=0) uniform sampler samp;

layout(location=0) out vec4 color;

void main() {
	color = vec4(1, 0, 0, 1);
}
