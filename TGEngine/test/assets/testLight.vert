#version 460

layout(location=0) in vec4 COLOR_0;
layout(location=1) in vec3 NORMAL;
layout(location=2) in vec3 POSITION;
layout(location=3) in vec2 TEXCOORD_0;

layout(location=0) out vec2 UVOUT;

layout(binding=2) uniform MVP {
	mat4 matrix;
} mvp;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	UVOUT = TEXCOORD_0;
	gl_Position = mvp.matrix * vec4(POSITION, 1);
}
