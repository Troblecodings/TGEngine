#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform format_block {
   vec2 multiplier;
} image_format_block;

layout(location = 0) in vec3 posIn;
layout(location = 1) in vec4 colorIn;

layout(location = 0) out vec4 colorOut;

out gl_PerVertex{
   vec4 gl_Position;
};

void main(){
    gl_Position = vec4(posIn.x * image_format_block.multiplier.x, posIn.y * image_format_block.multiplier.y, posIn.z, 1);
    colorOut = colorIn;
}
