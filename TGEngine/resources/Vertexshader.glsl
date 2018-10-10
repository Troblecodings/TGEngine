#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform format_block {
   vec2 multiplier;
} image_format_block;

layout(binding = 2) uniform mat_block{
    mat4 rotation_matrix;
} matrix_block;

layout(location = 0) in vec3 posIn;
layout(location = 1) in vec4 colorIn;
layout(location = 2) in vec2 uv;
layout(location = 3) in uint color_only;

layout(location = 0) out vec4 colorOut;
layout(location = 1) out vec2 uvOut;
layout(location = 2) flat out uint only_color;

out gl_PerVertex{
   vec4 gl_Position;
};

void main(){
    gl_Position = matrix_block.rotation_matrix * vec4(posIn, 1);
    colorOut = colorIn;
    uvOut = uv;
    only_color = color_only;
}
