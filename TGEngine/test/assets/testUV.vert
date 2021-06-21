#version 460

layout(location=0) in vec2 uv;
layout(location=1) in vec4 inpos;

layout(location=0) out vec2 uvOut;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	gl_Position = inpos;
	uvOut = uv;
}
