#pragma once

#include "Memory.hpp"
#include "../Descriptors.hpp"

namespace tge::buf {

	constexpr uint32_t TRANSFORM_BUFFER = 0;

	extern BufferObject buffers;

	void initUniformBuffers();

	void fillUniformBuffer(uint32_t uniformBufferIndex, void* data, VkDeviceSize size, VkDeviceSize offset = 0);

}