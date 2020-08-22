#pragma once

#include "../../../public/pipeline/buffer/Memory.hpp"
#include "../../../public/pipeline/Descriptors.hpp"

namespace tge::buf {

	constexpr uint32_t TRANSFORM_BUFFER = 0;
	constexpr uint32_t TRANSFORM_BUFFER_2 = 1;

	constexpr uint32_t UBO_COUNT = 2;

	extern BufferObject buffers[UBO_COUNT];

	void initUniformBuffers();

	void fillUniformBuffer(uint32_t uniformBufferIndex, void* data, VkDeviceSize size, VkDeviceSize offset = 0);

}
