#include "ShaderPipes.hpp"
// Auto generated / Don't change

VkPipelineShaderStageCreateInfo TexturedBasicShader[2];
const VkVertexInputAttributeDescription TexturedBasicInput[] = {{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT,0},{ 1, 0, VK_FORMAT_R32G32_SFLOAT,12},{ 2, 0, VK_FORMAT_R32G32B32_SFLOAT,20}};
const VkDescriptorSetLayoutBinding TexturedBasicLayoutBinding[] = {{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,VK_SHADER_STAGE_VERTEX_BIT},{ 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,VK_SHADER_STAGE_FRAGMENT_BIT},{ 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1,VK_SHADER_STAGE_FRAGMENT_BIT}};
const unsigned int TexturedBasicShaderCount = 2;
const unsigned int TexturedBasicInputCount = 3;
const unsigned int TexturedBasicLayoutBindingCount = 3;
ShaderPipe TexturedBasicPipe;


void initShaderPipes(){
    TexturedBasicShader[0] = VertexTextured;
    TexturedBasicShader[1] = FragmentTextured;
	TexturedBasicPipe = ShaderPipe(TexturedBasicShader, TexturedBasicInput, TexturedBasicLayoutBinding, TexturedBasicShaderCount, TexturedBasicInputCount, TexturedBasicLayoutBindingCount);
}
