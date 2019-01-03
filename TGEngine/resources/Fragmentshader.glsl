#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) flat in uint index;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec4 colorOut;

layout(binding = 0) uniform sampler2D image_sampler[4094];

void main(){
    if(index > 4094){
        colorOut = color;
        return;
    }
    colorOut = texture(image_sampler[index], uv) * color;
}