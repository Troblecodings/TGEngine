#version 460
#extension GL_ARB_separate_shader_objects : enable

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
    posIn = point_for_rotation(posIn.xy, posIn.z, camera_block.z, vec2(camera_block.origin_x, camera_block.origin_y));
    posIn = point_for_rotation(posIn.xz, posIn.y, camera_block.y, vec2(camera_block.origin_x, camera_block.origin_z));
    posIn = point_for_rotation(posIn.zy, posIn.x, camera_block.x, vec2(camera_block.origin_z, camera_block.origin_y));

    gl_Position = vec4(posIn, 0);
    colorOut = colorIn;  
}

vec3 point_for_rotation(vec2 xy, float z, float rotation, vec2 origin){
    radius = length(xy, origin);
    angle = asin(xy.x - origin.x/radius) + rotation;
    return vec3(origin.x + radius * cos(angle), origin.y + radius * sin(angle))
}