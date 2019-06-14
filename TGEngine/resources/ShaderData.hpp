#pragma once
#include "..\ShaderCreation.hpp"

void initShader();

extern const unsigned char FragmentColorOnlyModule[];
extern VkPipelineShaderStageCreateInfo FragmentColorOnly;
extern const unsigned char FragmentTexturedModule[];
extern VkPipelineShaderStageCreateInfo FragmentTextured;
extern const unsigned char FragmentUIColorModule[];
extern VkPipelineShaderStageCreateInfo FragmentUIColor;
extern const unsigned char FragmentUITexturedModule[];
extern VkPipelineShaderStageCreateInfo FragmentUITextured;
extern const unsigned char VertexColorOnlyModule[];
extern VkPipelineShaderStageCreateInfo VertexColorOnly;
extern const unsigned char VertexTexturedModule[];
extern VkPipelineShaderStageCreateInfo VertexTextured;
extern const unsigned char VertexUIModule[];
extern VkPipelineShaderStageCreateInfo VertexUI;
