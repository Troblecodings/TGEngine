#version 460
#extension GL_ARB_separate_shader_objects : enable

struct Light {
    vec4 pos;
    vec4 intensity;
};

layout(binding = 1) uniform LIGHTBLOCK {
   Light lights[1];
} lights;

layout(binding = 2) uniform sampler2D imageSampler;

layout(location = 0) in vec4 color;
layout(location = 1) in vec2 uv;

layout(location = 0) out vec4 colorOut;

// Static color
vec4 getColor() {
    return texture(imageSampler, uv) * color;
}

// Clamps the value within a certain value
float window(float dist, Light l) {
    return pow(max(1 - pow(dist / (l.intensity.w * 20), 4), 0), 2);
}

// Fall off function
float fall(float dist) {
    return pow(5 / max(dist, 1), 2);
}

// Light calculation with at least 1 distance
vec4 applyLight(vec3 lightPower) {
    vec4 curentColor = getColor();
    return vec4(vec3(curentColor.rgb * lightPower), curentColor.a);
}

void main(){
//  vec3 lightMultiplier = vec3(0);
//    for (uint i = 0; i < maxlights; ++i) {
//        Light light = lights.lights[i];
//        vec3 delta = light.pos.xyz - pos;
//        float dist = sqrt(dot(delta, delta));
//        vec3 lightM = light.intensity.xyz * (fall(dist) * window(dist, light) * clamp(light.intensity.w, 0, 2));
//        lightMultiplier += lightM;
//    }
    colorOut = getColor();
}
