#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) flat in uint index;

struct DATA {
uint sampler_index;
uint uv_index;
uint color_index;
} data;

layout(binding = 0) uniform sampler2D image_sampler[4094];
layout(binding = 1) uniform UV_BLOCK{ 
    vec2 uv_arr[65536];
} uv_block;
layout(binding = 2) uniform COLOR_BLOCK { 
    vec4 color_arr[65536];
} color_block;
layout(binding = 3) uniform DATA_BLOCK {
    DATA data_arr[65536]; // 2^16 vertex
} data_block;

layout(location = 0) out vec4 colorOut;

void main(){
    if(data_block.data_arr[index].sampler_index > 4094){
        colorOut = color_block.color_arr[data_block.data_arr[index].color_index];
        return;
    }
    colorOut = texture(image_sampler[data_block.data_arr[index].sampler_index], uv_block.uv_arr[data_block.data_arr[index].uv_index]) * color_block.color_arr[data_block.data_arr[index].color_index];
}