#version 460
#extension GL_ARB_separate_shader_objects : enable


layout(binding = 0) uniform DATA_BLOCK {
    mat4 basicTransform;
    mat4 localTransforms[4096];
} data;

layout(location = 0) in vec3 pos;
layout(location = 1) in uint id;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec2 uvOut;
layout(location = 1) out uint idOut;

out gl_PerVertex{
   vec4 gl_Position;
};

void main(){
    gl_Position = (vec4(pos, 1) * data.basicTransform) * data.localTransforms[id];
    idOut = id;
    uvOut = uv;
}