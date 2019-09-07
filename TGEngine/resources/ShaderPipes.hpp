#pragma once
#include "ShaderData.hpp"
// Auto generated / Don't change

extern VkPipelineShaderStageCreateInfo ColorOnlyNormalShader[2];
extern const VkVertexInputAttributeDescription ColorOnlyNormalInput[];
extern const VkDescriptorSetLayoutBinding ColorOnlyNormalLayoutBinding[];
extern const unsigned int ColorOnlyNormalShaderCount;
extern const unsigned int ColorOnlyNormalInputCount;
extern const unsigned int ColorOnlyNormalLayoutBindingCount;
extern ShaderPipe ColorOnlyNormalPipe;

extern VkPipelineShaderStageCreateInfo TexturedBasicShader[2];
extern const VkVertexInputAttributeDescription TexturedBasicInput[];
extern const VkDescriptorSetLayoutBinding TexturedBasicLayoutBinding[];
extern const unsigned int TexturedBasicShaderCount;
extern const unsigned int TexturedBasicInputCount;
extern const unsigned int TexturedBasicLayoutBindingCount;
extern ShaderPipe TexturedBasicPipe;

extern VkPipelineShaderStageCreateInfo UIColorShader[2];
extern const VkVertexInputAttributeDescription UIColorInput[];
extern const unsigned int UIColorShaderCount;
extern const unsigned int UIColorInputCount;
extern const unsigned int UIColorLayoutBindingCount;
extern ShaderPipe UIColorPipe;

extern VkPipelineShaderStageCreateInfo UITexturedShader[2];
extern const VkVertexInputAttributeDescription UITexturedInput[];
extern const VkDescriptorSetLayoutBinding UITexturedLayoutBinding[];
extern const unsigned int UITexturedShaderCount;
extern const unsigned int UITexturedInputCount;
extern const unsigned int UITexturedLayoutBindingCount;
extern ShaderPipe UITexturedPipe;


void initShaderPipes();
