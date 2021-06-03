#version 460

layout(binding=0) uniform sampler2D samp[2];
layout(binding=0) uniform test {
vec2 m;
};
layout(binding=0) uniform texture2D tex[16];

layout(location=0) out vec4 color;

void main() {
	color = vec4(1, 0, 0, 1);
}
