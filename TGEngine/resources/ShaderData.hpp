#pragma once
#include "../pipeline/ShaderCreation.hpp"
void initShader();

extern unsigned char FragmentColorOnlyModule[];
extern VkPipelineShaderStageCreateInfo FragmentColorOnly;
extern unsigned char FragmentTexturedModule[];
extern VkPipelineShaderStageCreateInfo FragmentTextured;
extern unsigned char FragmentTextured2DModule[];
extern VkPipelineShaderStageCreateInfo FragmentTextured2D;
extern unsigned char FragmentUIColorModule[];
extern VkPipelineShaderStageCreateInfo FragmentUIColor;
extern unsigned char FragmentUITexturedModule[];
extern VkPipelineShaderStageCreateInfo FragmentUITextured;
extern unsigned char VertexTexturedModule[];
extern VkPipelineShaderStageCreateInfo VertexTextured;
extern unsigned char VertexTextured2DModule[];
extern VkPipelineShaderStageCreateInfo VertexTextured2D;
extern unsigned char VertexTexturedMovableModule[];
extern VkPipelineShaderStageCreateInfo VertexTexturedMovable;
extern unsigned char VertexUIModule[];
extern VkPipelineShaderStageCreateInfo VertexUI;
