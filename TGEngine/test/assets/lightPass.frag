#version 450
#extension GL_KHR_vulkan_glsl: enable

layout(input_attachment_index=0, set=0, binding = 0) uniform subpassInput ALBEDO;
layout(input_attachment_index=1, set=0, binding = 1) uniform subpassInput NORMAL;
layout(input_attachment_index=2, set=0, binding = 2) uniform subpassInput ROUGHNESS;
layout(input_attachment_index=3, set=0, binding = 3) uniform subpassInput METALLIC;

layout(set=0, binding = 4) uniform _TMP {
    vec4 light[50];
    int lightCount;
} lights;

layout(location=0) out vec4 colorout;

void main() {
    vec3 color = subpassLoad(ALBEDO).rgb;
    vec3 normal = subpassLoad(NORMAL).rgb;
    float multiplier = 0;
    for(int x = 0; x < lights.lightCount; x++) {
        vec4 lightInfo = lights.light[x];
        multiplier += dot(normal, normalize(lightInfo.xyz)) * lightInfo.w;
    }
    colorout = vec4(color * multiplier, 1);
}
