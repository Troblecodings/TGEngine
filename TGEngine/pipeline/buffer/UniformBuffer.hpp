#pragma once

#include "Memory.hpp"
#include "../Descriptors.hpp"

class UniformBuffer {

public:

	INTERNAL
		SINCE(0, 0, 4)
		UniformBuffer() {} // Default constructor
	UniformBuffer(uint32_t size, VkShaderStageFlags flags) : UniformBuffer(size, flags, 0) {}
	UniformBuffer(uint32_t size, VkShaderStageFlags flags, uint32_t binding);

	uint32_t size = 0;
	Descriptor descriptor = Descriptor();
	uint32_t index = 0;
	void* memory = 0;

	void updateDescriptor();
};

SINCE(0, 0, 2)
void fillUniformBuffer(UniformBuffer* buffer, void* input, uint32_t size);