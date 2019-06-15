#pragma once
#include "ShaderData.hpp"
// Auto generated / Don't change

extern VkPipelineShaderStageCreateInfo ColorBasicShader[2];
extern const VkVertexInputAttributeDescription ColorBasicInput[];
extern const unsigned int ColorBasicShaderCount;
extern const unsigned int ColorBasicInputCount;

extern VkPipelineShaderStageCreateInfo TexturedBasicShader[2];
extern const VkVertexInputAttributeDescription TexturedBasicInput[];
extern const unsigned int TexturedBasicShaderCount;
extern const unsigned int TexturedBasicInputCount;

extern VkPipelineShaderStageCreateInfo UIColorShader[2];
extern const VkVertexInputAttributeDescription UIColorInput[];
extern const unsigned int UIColorShaderCount;
extern const unsigned int UIColorInputCount;

extern VkPipelineShaderStageCreateInfo UITexturedShader[2];
extern const VkVertexInputAttributeDescription UITexturedInput[];
extern const unsigned int UITexturedShaderCount;
extern const unsigned int UITexturedInputCount;


void initShaderPipes();
