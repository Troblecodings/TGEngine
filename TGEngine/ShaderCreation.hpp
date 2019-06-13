#pragma once

#include "Stdbase.hpp"
#include "pipeline/Device.hpp"

struct ShaderPipe {
	VkPipelineShaderStageCreateInfo* shader;
	uint32_t shaderCount;
	VkVertexInputAttributeDescription* vertexInputDescription;
	uint32_t vertexInputDescriptionCount;
};

VkPipelineShaderStageCreateInfo createShader(unsigned char data[], VkShaderStageFlagBits flag, uint32_t size);

ShaderPipe createShaderPipe(VkPipelineShaderStageCreateInfo shaderModule[], uint32_t shaderCount, VkVertexInputAttributeDescription vertexInputDescription[], uint32_t vertexInputDescriptionCount);