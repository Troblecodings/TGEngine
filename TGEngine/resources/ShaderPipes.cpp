#include "ShaderPipes.hpp"
// Auto generated / Don't change

VkPipelineShaderStageCreateInfo ColorOnlyNormalShader[2];
const VkVertexInputAttributeDescription ColorOnlyNormalInput[] = {{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT,0},{ 1, 0, VK_FORMAT_R32G32_SFLOAT,12},{ 2, 0, VK_FORMAT_R32G32B32_SFLOAT,20}};
const VkDescriptorSetLayoutBinding ColorOnlyNormalLayoutBinding[] = {{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,VK_SHADER_STAGE_VERTEX_BIT},{ 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,VK_SHADER_STAGE_FRAGMENT_BIT}};
const unsigned int ColorOnlyNormalShaderCount = 2;
const unsigned int ColorOnlyNormalInputCount = 3;
const unsigned int ColorOnlyNormalLayoutBindingCount = 2;
ShaderPipe ColorOnlyNormalPipe;

VkPipelineShaderStageCreateInfo Tex2DShader[2];
const VkVertexInputAttributeDescription Tex2DInput[] = {{ 0, 0, VK_FORMAT_R32G32_SFLOAT,0},{ 1, 0, VK_FORMAT_R32G32_SFLOAT,8},{ 2, 0, VK_FORMAT_R32G32B32A32_SFLOAT,16}};
const VkDescriptorSetLayoutBinding Tex2DLayoutBinding[] = {{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,VK_SHADER_STAGE_VERTEX_BIT},{ 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,VK_SHADER_STAGE_FRAGMENT_BIT},{ 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1,VK_SHADER_STAGE_FRAGMENT_BIT}};
const unsigned int Tex2DShaderCount = 2;
const unsigned int Tex2DInputCount = 3;
const unsigned int Tex2DLayoutBindingCount = 3;
ShaderPipe Tex2DPipe;

VkPipelineShaderStageCreateInfo TexturedBasicShader[2];
const VkVertexInputAttributeDescription TexturedBasicInput[] = {{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT,0},{ 1, 0, VK_FORMAT_R32G32_SFLOAT,12},{ 2, 0, VK_FORMAT_R32G32B32_SFLOAT,20}};
const VkDescriptorSetLayoutBinding TexturedBasicLayoutBinding[] = {{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,VK_SHADER_STAGE_VERTEX_BIT},{ 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,VK_SHADER_STAGE_FRAGMENT_BIT},{ 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1,VK_SHADER_STAGE_FRAGMENT_BIT}};
const unsigned int TexturedBasicShaderCount = 2;
const unsigned int TexturedBasicInputCount = 3;
const unsigned int TexturedBasicLayoutBindingCount = 3;
ShaderPipe TexturedBasicPipe;

VkPipelineShaderStageCreateInfo TopDownShader[2];
const VkVertexInputAttributeDescription TopDownInput[] = {{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT,0},{ 1, 0, VK_FORMAT_R32_UINT,12},{ 2, 0, VK_FORMAT_R32G32_SFLOAT,16}};
const VkDescriptorSetLayoutBinding TopDownLayoutBinding[] = {{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,VK_SHADER_STAGE_VERTEX_BIT},{ 1, VK_DESCRIPTOR_TYPE_SAMPLER, 1,VK_SHADER_STAGE_FRAGMENT_BIT},{ 2, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1,VK_SHADER_STAGE_FRAGMENT_BIT},{ 3, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1,VK_SHADER_STAGE_FRAGMENT_BIT}};
const unsigned int TopDownShaderCount = 2;
const unsigned int TopDownInputCount = 3;
const unsigned int TopDownLayoutBindingCount = 4;
ShaderPipe TopDownPipe;

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
    Tex2DShader[0] = VertexTextured2D;
    Tex2DShader[1] = FragmentTextured2D;
    Tex2DPipe = ShaderPipe(Tex2DShader, Tex2DInput, Tex2DLayoutBinding, Tex2DShaderCount, Tex2DInputCount, Tex2DLayoutBindingCount);
    TexturedBasicShader[0] = VertexTextured;
    TexturedBasicShader[1] = FragmentTextured;
    TexturedBasicPipe = ShaderPipe(TexturedBasicShader, TexturedBasicInput, TexturedBasicLayoutBinding, TexturedBasicShaderCount, TexturedBasicInputCount, TexturedBasicLayoutBindingCount);
    TopDownShader[0] = VertexTopDown;
    TopDownShader[1] = FragmentTopDown;
    TopDownPipe = ShaderPipe(TopDownShader, TopDownInput, TopDownLayoutBinding, TopDownShaderCount, TopDownInputCount, TopDownLayoutBindingCount);
    UIColorShader[0] = VertexUI;
    UIColorShader[1] = FragmentUIColor;
    UIColorPipe = ShaderPipe(UIColorShader, UIColorInput, nullptr, UIColorShaderCount, UIColorInputCount, UIColorLayoutBindingCount);
    UITexturedShader[0] = VertexUI;
    UITexturedShader[1] = FragmentUITextured;
    UITexturedPipe = ShaderPipe(UITexturedShader, UITexturedInput, UITexturedLayoutBinding, UITexturedShaderCount, UITexturedInputCount, UITexturedLayoutBindingCount);
}
