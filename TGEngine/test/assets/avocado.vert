#version 460

layout(location=0) in vec4 TAN;
layout(location=1) in vec3 NORMAL;
layout(location=2) in vec3 POSIN;
layout(location=3) in vec2 UV;

layout(location=0) out vec2 UVOUT;

layout(binding=2) uniform MVP {
	mat4 matrix;
} mvp;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	UVOUT = UV;
	gl_Position = mvp.matrix * vec4(POSIN, 1);
}
