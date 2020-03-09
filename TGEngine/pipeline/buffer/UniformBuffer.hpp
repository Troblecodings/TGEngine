#pragma once

#include "Memory.hpp"
#include "../Descriptors.hpp"

namespace tge::buf {

	constexpr uint32_t TRANSFORM_BUFFER = 0;
	constexpr uint32_t TRANSFORM_BUFFER_2 = 1;

	extern BufferObject buffers[3];

	void initUniformBuffers();

	void fillUniformBuffer(uint32_t uniformBufferIndex, void* data, VkDeviceSize size, VkDeviceSize offset = 0);

}