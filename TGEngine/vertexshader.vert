#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (std140, binding = 0) uniform bufferVals {
     mat4 mvp;
} bufvals;

layout(location = 0)in vec4 colorin;
layout(location = 1)in vec4 pos;
layout(location = 0)out vec4 colorout;

out gl_PerVertex{
    vec4 gl_Position;
};

void main(){
    colorout = colorin;
    gl_Position = bufvals.mvp * pos;
}