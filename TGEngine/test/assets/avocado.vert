#version 460

layout(location=0) in vec4 TAN;
layout(location=1) in vec3 NORMAL;
layout(location=2) in vec3 POS;
layout(location=3) in vec2 UV;

layout(location=0) out vec2 UVOUT;

layout(binding=2) uniform TEST {
	mat4 matrix;
} test;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	gl_Position = test.matrix * vec4(POS, 1);
	UVOUT = UV;
}
