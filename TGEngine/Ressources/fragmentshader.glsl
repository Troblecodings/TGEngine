#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 colorIn;
layout(location = 1) in vec2 uv;
layout(location = 2) flat in uint color_only;

layout(binding = 1) uniform sampler2D image_sampler[100];
layout(push_constant) uniform pushConstants{
uint tex_index;
} push;

layout(location = 0) out vec4 colorOut;

void main(){
    if(push.tex_index == 101){
        colorOut = colorIn;
        return;
    }
    colorOut = texture(image_sampler[push.tex_index], uv) * colorIn;
}