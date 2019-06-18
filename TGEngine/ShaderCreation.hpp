#pragma once

#include "Stdbase.hpp"
#include "pipeline/Device.hpp"

class ShaderPipe {

private:
	VkPipelineShaderStageCreateInfo* shader;
	const VkVertexInputAttributeDescription* inputs;
	const VkDescriptorSetLayoutBinding* layouts;
	const unsigned int shaderCount;
	const unsigned int inputCount;
	const unsigned int layoutCount;

public:
	ShaderPipe(VkPipelineShaderStageCreateInfo* shader,
		const VkVertexInputAttributeDescription* inputs,
		const VkDescriptorSetLayoutBinding* layouts,
		const unsigned int shaderCount,
		const unsigned int inputCount,
		const unsigned int layoutCount) :
		shaderCount(shaderCount),
		inputCount(inputCount),
		layoutCount(layoutCount),
		shader(shader),
		inputs(inputs),
		layouts(layouts) {}
};
VkPipelineShaderStageCreateInfo createShader(const unsigned char data[], VkShaderStageFlagBits flag, uint32_t size);