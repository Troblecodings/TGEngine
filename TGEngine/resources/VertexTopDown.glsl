#version 460

layout(set = 1, binding = 0) uniform DATA_BLOCK {
    mat4 basicTransform;
    vec2 uvOffsets[256];
} data;

layout(push_constant) uniform PUSH_CONST{
    mat4 localTransform;
    uint animationIndex;
} pushconst;

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec2 uvOut;

out gl_PerVertex{
   vec4 gl_Position;
};

void main(){
    gl_Position = (vec4(pos, 1, 1) * data.basicTransform * pushconst.localTransform);
    uvOut = uv + data.uvOffsets[pushconst.animationIndex];
}