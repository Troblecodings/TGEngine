#pragma once

#include "Memory.hpp"
#include "../Descriptors.hpp"

class UniformBuffer {

public:

	INTERNAL
	SINCE(0, 0, 4)
	UniformBuffer() : UniformBuffer(0, 0) {} // Default constructor
	UniformBuffer(uint32_t size, VkShaderStageFlags flags) : UniformBuffer(size, flags, 0xFFFFFFFF) {}
	UniformBuffer(uint32_t size, VkShaderStageFlags flags, uint32_t buffer);

	uint32_t size;
	Descriptor descriptor;
	uint32_t index = 0;
	void* memory = 0;
};

SINCE(0, 0, 2)
void fillUniformBuffer(UniformBuffer* buffer, void* input, uint32_t size);