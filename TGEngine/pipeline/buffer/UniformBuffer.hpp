#pragma once

#include "Memory.hpp"
#include "../Descriptors.hpp"

struct UniformBuffer {
	INPUT
	uint32_t size;
	Descriptor descriptor;
	OUTPUT
	uint32_t index;
	void* memory;
};

SINCE(0, 0, 2)
void createUniformBuffer(UniformBuffer* buffer);

SINCE(0, 0, 2)
void fillUniformBuffer(UniformBuffer* buffer, void* input, uint32_t size);