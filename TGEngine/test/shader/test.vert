#version 460

layout(location=0) in vec4 inpos;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	gl_Position = inpos;
}
