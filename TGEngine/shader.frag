#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0)out vec4 color;

vec3 colors[3] = vec3[](
vec3(1.0f,1.0f,1.0f),
vec3(1.0f,0.0f,1.0f),
vec3(0.0f,1.0f,1.0f)
);

void main(){
   color = vec4(colors[0],1.0);
}