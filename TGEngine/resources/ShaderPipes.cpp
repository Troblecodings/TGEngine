#include "ShaderPipes.hpp"
// Auto generated / Don't change

VkPipelineShaderStageCreateInfo ColorOnlyNormalShader[2];
const VkVertexInputAttributeDescription ColorOnlyNormalInput[] = {{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT,0},{ 1, 0, VK_FORMAT_R32G32_SFLOAT,12},{ 2, 0, VK_FORMAT_R32G32B32_SFLOAT,20}};
const VkDescriptorSetLayoutBinding ColorOnlyNormalLayoutBinding[] = {{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,VK_SHADER_STAGE_VERTEX_BIT},{ 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,VK_SHADER_STAGE_FRAGMENT_BIT}};
const unsigned int ColorOnlyNormalShaderCount = 2;
const unsigned int ColorOnlyNormalInputCount = 3;
const unsigned int ColorOnlyNormalLayoutBindingCount = 2;
ShaderPipe ColorOnlyNormalPipe;

VkPipelineShaderStageCreateInfo TexturedBasicShader[2];
const VkVertexInputAttributeDescription TexturedBasicInput[] = {{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT,0},{ 1, 0, VK_FORMAT_R32G32_SFLOAT,12},{ 2, 0, VK_FORMAT_R32G32B32_SFLOAT,20}};
const VkDescriptorSetLayoutBinding TexturedBasicLayoutBinding[] = {{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,VK_SHADER_STAGE_VERTEX_BIT},{ 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,VK_SHADER_STAGE_FRAGMENT_BIT},{ 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1,VK_SHADER_STAGE_FRAGMENT_BIT}};
const unsigned int TexturedBasicShaderCount = 2;
const unsigned int TexturedBasicInputCount = 3;
const unsigned int TexturedBasicLayoutBindingCount = 3;
ShaderPipe TexturedBasicPipe;

VkPipelineShaderStageCreateInfo UIColorShader[2];
const VkVertexInputAttributeDescription UIColorInput[] = {{ 0, 0, VK_FORMAT_R32G32_SFLOAT,0},{ 1, 0, VK_FORMAT_R32G32_SFLOAT,8},{ 2, 0, VK_FORMAT_R32G32B32A32_SFLOAT,16}};
const unsigned int UIColorShaderCount = 2;
const unsigned int UIColorInputCount = 3;
const unsigned int UIColorLayoutBindingCount = 0;
ShaderPipe UIColorPipe;

VkPipelineShaderStageCreateInfo UITexturedShader[2];
const VkVertexInputAttributeDescription UITexturedInput[] = {{ 0, 0, VK_FORMAT_R32G32_SFLOAT,0},{ 1, 0, VK_FORMAT_R32G32_SFLOAT,8},{ 2, 0, VK_FORMAT_R32G32B32A32_SFLOAT,16}};
const VkDescriptorSetLayoutBinding UITexturedLayoutBinding[] = {{ 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1,VK_SHADER_STAGE_FRAGMENT_BIT}};
const unsigned int UITexturedShaderCount = 2;
const unsigned int UITexturedInputCount = 3;
const unsigned int UITexturedLayoutBindingCount = 1;
ShaderPipe UITexturedPipe;


void initShaderPipes(){
    ColorOnlyNormalShader[0] = VertexTextured;
    ColorOnlyNormalShader[1] = FragmentColorOnly;
    ColorOnlyNormalPipe = ShaderPipe(ColorOnlyNormalShader, ColorOnlyNormalInput, ColorOnlyNormalLayoutBinding, ColorOnlyNormalShaderCount, ColorOnlyNormalInputCount, ColorOnlyNormalLayoutBindingCount);
    TexturedBasicShader[0] = VertexTextured;
    TexturedBasicShader[1] = FragmentTextured;
    TexturedBasicPipe = ShaderPipe(TexturedBasicShader, TexturedBasicInput, TexturedBasicLayoutBinding, TexturedBasicShaderCount, TexturedBasicInputCount, TexturedBasicLayoutBindingCount);
    UIColorShader[0] = VertexUI;
    UIColorShader[1] = FragmentUIColor;
    UIColorPipe = ShaderPipe(UIColorShader, UIColorInput, nullptr, UIColorShaderCount, UIColorInputCount, UIColorLayoutBindingCount);
    UITexturedShader[0] = VertexUI;
    UITexturedShader[1] = FragmentUITextured;
    UITexturedPipe = ShaderPipe(UITexturedShader, UITexturedInput, UITexturedLayoutBinding, UITexturedShaderCount, UITexturedInputCount, UITexturedLayoutBindingCount);
}
