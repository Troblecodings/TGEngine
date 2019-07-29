#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include "Device.hpp"

class ShaderPipe {

public:
	unsigned int descriptorLayout;
	unsigned int layoutIndex;
	VkPipelineShaderStageCreateInfo* shader;

private:
	VkVertexInputAttributeDescription* inputs;
	VkDescriptorSetLayoutBinding* layouts;
	unsigned int shaderCount;
	unsigned int inputCount;
	unsigned int layoutCount;

public:
	ShaderPipe() {}
	ShaderPipe(VkPipelineShaderStageCreateInfo* shader,
		const VkVertexInputAttributeDescription* inputs,
		const VkDescriptorSetLayoutBinding* layouts,
		const unsigned int shaderCount,
		const unsigned int inputCount,
		const unsigned int layoutCount);

	void precreation();
};

VkPipelineShaderStageCreateInfo createShader(const unsigned char data[], VkShaderStageFlagBits flag, uint32_t size);