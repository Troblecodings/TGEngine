#include "ShaderPipes.hpp"

ShaderPipe TexturedBasic;

void initShaderPipes() {
    TexturedBasic = createShaderPipe(new VkPipelineShaderStageCreateInfo[]{VertexTextured,FragmentTextured},2,new VkVertexInputAttributeDescription[]{{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT,0},{ 1, 0, VK_FORMAT_R32G32_SFLOAT,12},{ 2, 0, VK_FORMAT_R32G32B32_SFLOAT,20}},3);
}
