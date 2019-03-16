#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform MATRIX_BLOCK{
    mat4 matrix;
} matrix_block;
layout(binding = 2) uniform LIGHT_BLOCK{
    vec3 light;
} light_block;

layout(location = 0) in vec3 posIn;
layout(location = 1) in vec4 colorIn;
layout(location = 2) in vec2 uv;
layout(location = 3) in uint indx;
layout(location = 4) in vec3 normalIn;

layout(location = 0) flat out uint index_id;
layout(location = 1) out vec4 colorOut;
layout(location = 2) out vec2 uvOut;

out gl_PerVertex{
   vec4 gl_Position;
};

void main(){
    gl_Position = matrix_block.matrix * vec4(posIn, 1);
	index_id = indx;
	uvOut = uv;

	vec3 to_light = normalize(light_block.light);
	vec3 vnormal = normalize(normalIn);

	float cos_angle = clamp(dot(to_light, vnormal), 0.0, 1.0);
	colorOut = vec4(vec3(colorIn) * cos_angle, colorIn.a);
}
