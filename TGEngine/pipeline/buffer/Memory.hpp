#pragma once

#include "../../pipeline/Device.hpp"
#include "../../vlib/VulkanBuffer.hpp"

namespace tge::buf {

	/*
	 * This holds the information about a buffer
	 * <ul>
	 * <li>flags;
	 * <li>size;
	 * <li>memoryIndex;
	 * <li>bufferUsageFlag;
	 */
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
