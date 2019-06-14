#include "ShaderPipes.hpp"
// Auto generated / Don't change

VkPipelineShaderStageCreateInfo ColorBasicShader[2];
const VkVertexInputAttributeDescription ColorBasicInput[] = {{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT,0},{ 1, 0, VK_FORMAT_R32G32_SFLOAT,12},{ 2, 0, VK_FORMAT_R32G32B32_SFLOAT,20}};
const unsigned int ColorBasicShaderCount = 2;
const unsigned int ColorBasicInputCount = 3;

VkPipelineShaderStageCreateInfo TexturedBasicShader[2];
const VkVertexInputAttributeDescription TexturedBasicInput[] = {{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT,0},{ 1, 0, VK_FORMAT_R32G32_SFLOAT,12},{ 2, 0, VK_FORMAT_R32G32B32_SFLOAT,20}};
const unsigned int TexturedBasicShaderCount = 2;
const unsigned int TexturedBasicInputCount = 3;

VkPipelineShaderStageCreateInfo UIColorShader[2];
const VkVertexInputAttributeDescription UIColorInput[] = {{ 0, 0, VK_FORMAT_R32G32_SFLOAT,0},{ 1, 0, VK_FORMAT_R32G32_SFLOAT,8},{ 2, 0, VK_FORMAT_R32G32B32A32_SFLOAT,16}};
const unsigned int UIColorShaderCount = 2;
const unsigned int UIColorInputCount = 3;

VkPipelineShaderStageCreateInfo UITexturedShader[2];
const VkVertexInputAttributeDescription UITexturedInput[] = {{ 0, 0, VK_FORMAT_R32G32_SFLOAT,0},{ 1, 0, VK_FORMAT_R32G32_SFLOAT,8},{ 2, 0, VK_FORMAT_R32G32B32A32_SFLOAT,16}};
const unsigned int UITexturedShaderCount = 2;
const unsigned int UITexturedInputCount = 3;


void initShaderPipes(){
    ColorBasicShader[0] = VertexColorOnly;
    ColorBasicShader[1] = FragmentColorOnly;

    TexturedBasicShader[0] = VertexTextured;
    TexturedBasicShader[1] = FragmentTextured;

    UIColorShader[0] = VertexUI;
    UIColorShader[1] = FragmentUIColor;

    UITexturedShader[0] = VertexUI;
    UITexturedShader[1] = FragmentUITextured;

}
