#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex{
    vec4 gl_Position;
};

layout(constant_id = 1) const int arraySize = 5000;
in vec3 posses[arraySize];

void main(){
    gl_Position = vec4(posses[gl_VertexIndex],1.0);
}