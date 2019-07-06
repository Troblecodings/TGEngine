#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(constant_id = 0) const float r = 1;
layout(constant_id = 1) const float g = 1;
layout(constant_id = 2) const float b = 1;
layout(constant_id = 3) const float a = 1;

layout(binding = 1) uniform LIGHT_BLOCK{
    vec3 light;
} light_block;

layout(binding = 2) uniform sampler2D image_sampler;

layout(location = 0) in vec3 normalIn;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 pos;

layout(location = 0) out vec4 colorOut;

const float MAX_FALLOFF = 50;

// Static color
vec4 getColor() {
    return texture(image_sampler, uv) * vec4(r, g, b, a);
}

// Clamps the value within a certain value
float window(float dist) {
    return pow(max(1 - pow(dist / MAX_FALLOFF, 4), 0), 2);
}

// Fall off function
float fall(float dist) {
    return pow(5 / max(dist, 1.5), 2);
}

// Light calculation with at least 1 distance
vec4 applyLight(float dist) {
    float fallof = fall(dist) * window(dist);
    vec4 Ccolor = getColor();
    return vec4(vec3(Ccolor * fallof), Ccolor.a);
}

void main(){
    vec3 delta = light_block.light - pos;
    float dist = sqrt(dot(delta, delta));
    colorOut = applyLight(dist);
}