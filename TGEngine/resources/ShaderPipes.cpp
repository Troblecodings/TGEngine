#include "ShaderPipes.hpp"
// Auto generated / Don't change

VkPipelineShaderStageCreateInfo TexturedBasicShader[2];
const VkVertexInputAttributeDescription TexturedBasicInput[] = {{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT,0},{ 1, 0, VK_FORMAT_R32G32_SFLOAT,12},{ 2, 0, VK_FORMAT_R32G32B32_SFLOAT,20}};
const unsigned int TexturedBasicShaderCount = 2;
const unsigned int TexturedBasicInputCount = 3;


void initShaderPipes(){
    TexturedBasicShader[0] = VertexTextured;
    TexturedBasicShader[1] = FragmentTextured;

}
