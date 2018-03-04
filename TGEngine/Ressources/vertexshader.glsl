#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform format_block {
   vec2 multiplier;
} image_format_block;

layout(binding = 1) uniform camera {
    float x;
    float y;
    float z;
    float origin_x;
    float origin_y;
    float origin_z;
} camera_block;

layout(location = 0) in vec3 posIn;
layout(location = 1) in vec4 colorIn;

layout(location = 0) out vec4 colorOut;

out gl_PerVertex{
   vec4 gl_Position;
};

void main(){
    posIn = vec3(posIn.x * image_format_block.multiplier.x, posIn.y * image_format_block.multiplier.y, posIn.z)

    float z_radius = distance(posIn.xy, vec2(camera_block.origin_x, camera_block.origin_y));
    float z_angle = asin(posIn.y - origin.y/radius) + camera_block.z;
    posIn = vec3(origin.x + z_radius * cos(z_angle), origin.y + z_radius * sin(z_angle), posIn.z);

    float y_radius = distance(posIn.xz, vec2(camera_block.origin_x, camera_block.origin_z));
    float y_angle = asin(posIn.z - origin.z/radius) + camera_block.y;
    posIn = vec3(origin.x + y_radius * cos(y_angle), posIn.y, origin.z + y_radius * sin(y_angle));

    float x_radius = distance(posIn.zy, vec2(camera_block.origin_z, camera_block.origin_y));
    float x_angle = asin(posIn.y - origin.y/radius) + camera_block.x;
    posIn = vec3(posIn.x, origin.y + x_radius * sin(x_angle), origin.z + x_radius * cos(x_angle));

    gl_Position = vec4(posIn, 1);
    colorOut = colorIn;
}
