#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform MATRIX_BLOCK{
    mat4 matrix;
} matrix_block;

layout(location = 0) in vec3 posIn;
layout(location = 1) in vec4 colorIn;
layout(location = 2) in vec2 uv;
layout(location = 3) in uint indx;

layout(location = 0) flat out uint index_id;
layout(location = 1) out vec4 colorOut;
layout(location = 2) out vec2 uvOut;

out gl_PerVertex{
   vec4 gl_Position;
};

void main(){
    gl_Position = matrix_block.matrix * vec4(posIn, 1);
	colorOut = colorIn;
	index_id = indx;
	uvOut = uv;
}
