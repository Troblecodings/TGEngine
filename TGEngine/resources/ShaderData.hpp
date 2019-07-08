#pragma once
#include "../pipeline/ShaderCreation.hpp"
void initShader();

extern unsigned char FragmentColorOnlyModule[];
extern VkPipelineShaderStageCreateInfo FragmentColorOnly;
extern unsigned char FragmentTexturedModule[];
extern VkPipelineShaderStageCreateInfo FragmentTextured;
extern unsigned char FragmentUIColorModule[];
extern VkPipelineShaderStageCreateInfo FragmentUIColor;
extern unsigned char FragmentUITexturedModule[];
extern VkPipelineShaderStageCreateInfo FragmentUITextured;
extern unsigned char VertexColorOnlyModule[];
extern VkPipelineShaderStageCreateInfo VertexColorOnly;
extern unsigned char VertexTexturedModule[];
extern VkPipelineShaderStageCreateInfo VertexTextured;
extern unsigned char VertexTexturedMovableModule[];
extern VkPipelineShaderStageCreateInfo VertexTexturedMovable;
extern unsigned char VertexUIModule[];
extern VkPipelineShaderStageCreateInfo VertexUI;
