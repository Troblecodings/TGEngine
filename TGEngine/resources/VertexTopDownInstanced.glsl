#version 460

layout(set = 1, binding = 0) uniform DATA_BLOCK {
    mat4 basicTransform;
} data;

layout(push_constant) uniform PUSH_CONST{
    mat4 localTransform;
} pushconst;

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec2 uvOut;

layout(location = 2) in mat4 transform;

out gl_PerVertex{
   vec4 gl_Position;
};

void main(){
    gl_Position = (vec4(pos, 1, 1) * data.basicTransform * pushconst.localTransform);
    uvOut = uv;
}