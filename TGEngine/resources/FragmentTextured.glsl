#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform LIGHT_BLOCK{
    vec3 light;
} light_block;

layout(constant_id = 0) const float r = 1;
layout(constant_id = 1) const float g = 1;
layout(constant_id = 2) const float b = 1;
layout(constant_id = 3) const float a = 1;

layout(binding = 0) uniform sampler2D image_sampler;

layout(location = 0) in vec3 normalIn;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec4 colorOut;

void main(){
    colorOut = vec4(r, g, b, a);
    
    vec3 to_light = normalize(light_block.light);
	vec3 vnormal = normalize(normalIn);

	float cos_angle = clamp(dot(to_light, vnormal), 0.0, 1.0);
	colorOut = vec4(vec3(colorOut) * cos_angle, colorOut.a);

    colorOut = texture(image_sampler, uv) * colorOut;
}