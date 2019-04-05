#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform MATRIX_BLOCK{
    mat4 matrix;
} matrix_block;

layout(binding = 1) uniform LIGHT_BLOCK{
    vec3 light;
} light_block;

layout(location = 0) in vec3 posIn;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normalIn;

layout(location = 0) out float light_amplifier;
layout(location = 1) out vec2 uvOut;

out gl_PerVertex{
   vec4 gl_Position;
};

void main(){
    gl_Position = matrix_block.matrix * vec4(posIn, 1);
	uvOut = uv;

	vec3 to_light = normalize(posIn - light_block.light);
	vec3 vnormal = normalize(normalIn);

	light_amplifier = clamp(dot(to_light, vnormal), 0.0, 1.0);
}
