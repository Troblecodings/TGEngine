#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform MATRIX_BLOCK{
    mat4 matrix;
} matrix_block;

layout(location = 0) in vec3 posIn;
layout(location = 0) flat out uint index_id;

out gl_PerVertex{
   vec4 gl_Position;
};

void main(){
    gl_Position = matrix_block.matrix * vec4(posIn, 1);
	index_id = gl_VertexIndex;
}
