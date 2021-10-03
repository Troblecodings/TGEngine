#version 450
#extension GL_KHR_vulkan_glsl: enable

layout(input_attachment_index=0, set=0, binding = 0) uniform subpassInput ALBEDO;
layout(input_attachment_index=1, set=0, binding = 1) uniform subpassInput NORMAL;
layout(input_attachment_index=2, set=0, binding = 2) uniform subpassInput ROUGHNESS;
layout(input_attachment_index=3, set=0, binding = 3) uniform subpassInput METALLIC;
layout(input_attachment_index=4, set=0, binding = 5) uniform subpassInput POSITION;

struct Light {
    vec3 pos;
    vec3 color;
    float intensity;
};

layout(set=0, binding = 4) uniform _TMP {
    Light light[50];
    int lightCount;
} lights;

layout(location=0) out vec4 colorout;

void main() {
    vec3 color = subpassLoad(ALBEDO).rgb;
    vec3 normal = subpassLoad(NORMAL).rgb;
    vec3 pos = subpassLoad(POSITION).rgb;
    vec3 multiplier = vec3(0.08f, 0.08f, 0.08f);
    for(int x = 0; x < lights.lightCount; x++) {
        Light lightInfo = lights.light[x];
        vec3 l = normalize(lightInfo.pos);
        float refl = abs(dot(normal, l));
        multiplier += lightInfo.color * refl * lightInfo.intensity;
    }
    colorout = vec4(color * multiplier, 1);
}
