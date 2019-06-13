#pragma once
#include "..\ShaderCreation.hpp"

void initShader();

extern unsigned char FragmentColorOnlyModule[];
VkPipelineShaderStageCreateInfo FragmentColorOnly;
extern unsigned char FragmentTexturedModule[];
VkPipelineShaderStageCreateInfo FragmentTextured;
extern unsigned char FragmentUIColorModule[];
VkPipelineShaderStageCreateInfo FragmentUIColor;
extern unsigned char FragmentUITexturedModule[];
VkPipelineShaderStageCreateInfo FragmentUITextured;
extern unsigned char VertexColorOnlyModule[];
VkPipelineShaderStageCreateInfo VertexColorOnly;
extern unsigned char VertexTexturedModule[];
VkPipelineShaderStageCreateInfo VertexTextured;
extern unsigned char VertexUIModule[];
VkPipelineShaderStageCreateInfo VertexUI;
