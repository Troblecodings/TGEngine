#pragma once

#include "../../pipeline/Device.hpp"
#include "../../vlib/VulkanBuffer.hpp"

namespace tge::buf {

	struct BufferInputInfo {
		VkShaderStageFlags flags;
		uint32_t           size;
		uint32_t           memoryIndex;
		VkBufferUsageFlags bufferUsageFlag;
	};

	struct BufferObject {
		VkBuffer buffer;
		VkDeviceMemory memory;
	};

	void createBuffers(BufferInputInfo* inputInfo, uint32_t size, BufferObject* ubo);

}
