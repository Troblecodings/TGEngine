#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 posIn;
layout(location = 1) in vec3 colorIn;
layout(location = 0) out vec3 colorOut;

out gl_PerVertex{
   vec4 gl_Position;
};

void main(){
   gl_Position = vec4(posIn, 0 , 1);
   colorOut = colorIn;
}
