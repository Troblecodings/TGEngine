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

extern VkPipelineShaderStageCreateInfo Tex2DShader[2];
extern const VkVertexInputAttributeDescription Tex2DInput[];
extern const VkDescriptorSetLayoutBinding Tex2DLayoutBinding[];
extern const unsigned int Tex2DShaderCount;
extern const unsigned int Tex2DInputCount;
extern const unsigned int Tex2DLayoutBindingCount;
extern ShaderPipe Tex2DPipe;

extern VkPipelineShaderStageCreateInfo TexturedBasicShader[2];
extern const VkVertexInputAttributeDescription TexturedBasicInput[];
extern const VkDescriptorSetLayoutBinding TexturedBasicLayoutBinding[];
extern const unsigned int TexturedBasicShaderCount;
extern const unsigned int TexturedBasicInputCount;
extern const unsigned int TexturedBasicLayoutBindingCount;
extern ShaderPipe TexturedBasicPipe;

extern VkPipelineShaderStageCreateInfo TopDownInstancedShader[2];
extern const VkVertexInputAttributeDescription TopDownInstancedInput[];
extern const VkDescriptorSetLayoutBinding TopDownInstancedLayoutBinding[];
extern const unsigned int TopDownInstancedShaderCount;
extern const unsigned int TopDownInstancedInputCount;
extern const unsigned int TopDownInstancedLayoutBindingCount;
extern ShaderPipe TopDownInstancedPipe;

extern VkPipelineShaderStageCreateInfo TopDownShader[2];
extern const VkVertexInputAttributeDescription TopDownInput[];
extern const VkDescriptorSetLayoutBinding TopDownLayoutBinding[];
extern const unsigned int TopDownShaderCount;
extern const unsigned int TopDownInputCount;
extern const unsigned int TopDownLayoutBindingCount;
extern ShaderPipe TopDownPipe;

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
