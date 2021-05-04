#version 460

layout(location=0) in vec4 inpos;
layout(location=1) in vec2 incolor;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	gl_Position = inpos;
}
