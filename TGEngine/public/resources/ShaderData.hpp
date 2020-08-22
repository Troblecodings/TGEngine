#pragma once
#include "../../public/pipeline/ShaderCreation.hpp"
void initShader();
void destroyShader();

extern unsigned char FragmentColorOnlyModule[];
extern VkPipelineShaderStageCreateInfo FragmentColorOnly;
extern unsigned char FragmentTexturedModule[];
extern VkPipelineShaderStageCreateInfo FragmentTextured;
extern unsigned char FragmentTextured2DModule[];
extern VkPipelineShaderStageCreateInfo FragmentTextured2D;
extern unsigned char FragmentTopDownModule[];
extern VkPipelineShaderStageCreateInfo FragmentTopDown;
extern unsigned char FragmentUIColorModule[];
extern VkPipelineShaderStageCreateInfo FragmentUIColor;
extern unsigned char FragmentUITexturedModule[];
extern VkPipelineShaderStageCreateInfo FragmentUITextured;
extern unsigned char vertModule[];
extern VkPipelineShaderStageCreateInfo vert;
extern unsigned char VertexTexturedModule[];
extern VkPipelineShaderStageCreateInfo VertexTextured;
extern unsigned char VertexTextured2DModule[];
extern VkPipelineShaderStageCreateInfo VertexTextured2D;
extern unsigned char VertexTexturedMovableModule[];
extern VkPipelineShaderStageCreateInfo VertexTexturedMovable;
extern unsigned char VertexTopDownModule[];
extern VkPipelineShaderStageCreateInfo VertexTopDown;
extern unsigned char VertexTopDownInstancedModule[];
extern VkPipelineShaderStageCreateInfo VertexTopDownInstanced;
extern unsigned char VertexUIModule[];
extern VkPipelineShaderStageCreateInfo VertexUI;
