#version 460

layout(location=0) in vec4 TAN;
layout(location=1) in vec3 NORMAL;
layout(location=2) in vec3 POS;
layout(location=3) in vec2 UV;

layout(location=0) out vec2 UVOUT;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	gl_Position = vec4(POS, 1);
	UVOUT = UV;
}
