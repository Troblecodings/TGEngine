#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex{
    vec4 gl_Position;
};

vec3 posses[3] = vec3[](
vec3(1.0f,1.0f,1.0f),
vec3(1.0f,0.0f,1.0f),
vec3(0.0f,1.0f,1.0f)
);

void main(){
    gl_Position = vec4(posses[gl_VertexIndex],1.0);
}