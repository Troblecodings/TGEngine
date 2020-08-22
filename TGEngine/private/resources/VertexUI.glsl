#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 posIn;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 color;

layout(location = 0) out vec4 colorOut;
layout(location = 1) out vec2 uvOut;

out gl_PerVertex{
   vec4 gl_Position;
};

void main(){
    gl_Position = vec4(posIn, 1, 1);
    colorOut = color;
    uvOut = uv;
}
