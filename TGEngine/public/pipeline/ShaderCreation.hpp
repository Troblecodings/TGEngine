#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include "../../public/pipeline/Device.hpp"

struct ShaderPipe {

	unsigned int layoutIndex;
	VkPipelineShaderStageCreateInfo* shader;
	VkVertexInputAttributeDescription* inputs;
	VkDescriptorSetLayoutBinding* layouts;
	unsigned int shaderCount;
	unsigned int inputCount;
	unsigned int layoutCount;

	ShaderPipe() {}
	ShaderPipe(VkPipelineShaderStageCreateInfo* shader,
		VkVertexInputAttributeDescription* inputs,
		const VkDescriptorSetLayoutBinding* layouts,
		const unsigned int shaderCount,
		const unsigned int inputCount,
		const unsigned int layoutCount);

	void precreation();
};

VkPipelineShaderStageCreateInfo createShader(const unsigned char data[], VkShaderStageFlagBits flag, uint32_t size);