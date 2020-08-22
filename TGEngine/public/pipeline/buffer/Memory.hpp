#pragma once

#include "../../../public/pipeline/Device.hpp"
#include "../../../public/vlib/VulkanBuffer.hpp"

namespace tge::buf {

	/*
	 * Holds the information about a buffer. Used as an input for
	 * <strong class='atr'>createBuffers</strong>
	 * <ul>
	 * <li><strong class='atr'>flags</strong> is the stage in which to use the buffer</li>
	 * <li><strong class='atr'>size</strong> is the size of the buffers content</li>
	 * <li><strong class='atr'>memoryIndex</strong> is the index of the memory type in the queue</li>
	 * <li><strong class='atr'>bufferUsageFlag</strong></li> is the type of buffer as which it is used</ul>
	 * <h3>Valid usage</h3>
	 * <ul>
	 * <li><strong class='atr'>flags</strong> must be a valid VkShaderStageFlag</li>
	 * <li><strong class='atr'>size</strong> must be greater then 0</li>
	 * <li><strong class='atr'>memoryIndex</strong> must be valid memory index</li>
	 * <li><strong class='atr'>bufferUsageFlag</strong> must be a valid VkBufferUsageFlag</li></ul>
	 */
	struct BufferInputInfo {
		VkShaderStageFlags flags;
		uint32_t           size;
		uint32_t           memoryIndex;
		VkBufferUsageFlags bufferUsageFlag;
	};

	/*
	 * Output of <strong class='atr'>createBuffers</strong>
	 * <li><strong class='atr'>buffer</strong></li>
	 * <li><strong class='atr'>memory</strong></li>
	 */
	struct BufferObject {
		VkBuffer       buffer;
		VkDeviceMemory memory;
	};

	void createBuffers(BufferInputInfo* inputInfo, uint32_t size, BufferObject* ubo);

	void destroyBuffers(BufferObject* ubos, uint32_t size);

}
