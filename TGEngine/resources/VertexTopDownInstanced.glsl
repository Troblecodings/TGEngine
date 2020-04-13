#version 460

layout(set = 1, binding = 0) uniform DATA_BLOCK {
    mat4 basicTransform;
    vec4 transforms[128];
    vec2 uvOffsets[128];
} data;

layout(push_constant) uniform PUSH_CONST{
    mat4 localTransform;
    uint animationIndex;
    uint transformid;
} pushconst;

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec2 uvOut;

layout(location = 2) in vec2 transform;
layout(location = 3) in vec2 scale;
layout(location = 4) in uint lanimationIndex;
layout(location = 5) in uint ltransforms;

out gl_PerVertex{
   vec4 gl_Position;
};

void main(){
    mat4 matrix1 = {
        { scale.x, 0, 0, transform.x },
        { 0, scale.y, 0, transform.y },
        { 0, 0, 1.0, 0 },
        { 0, 0, 0, 1.0 }
    };
    vec4 pack = data.transforms[ltransforms];
    mat4 matrix2 = {
        { pack.x, 0, 0, pack.z },
        { 0, pack.y, 0, pack.w },
        { 0, 0, 1, 1 },
        { 0, 0, 0, 1.0 }
    };
    gl_Position = (vec4(pos, 1, 1) * data.basicTransform * pushconst.localTransform * matrix1 * matrix2);
    uvOut = uv + data.uvOffsets[lanimationIndex];
}