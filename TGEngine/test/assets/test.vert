#version 460

layout(location=0) in vec3 inpos;

layout(binding=2) uniform TEST {
	mat4 matrix;
} test;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	gl_Position = test.matrix * vec4(inpos, 1);
}
