#include "StagingBuffer.hpp"

std::vector<StagingBuffer*> staging_buffer;

void createStagingBuffer(StagingBuffer* buffer) {
	vlib_buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	vlib_buffer_create_info.size = buffer->size;
	lastResult = vkCreateBuffer(device, &vlib_buffer_create_info, nullptr, &buffer->staging_buffer);
	HANDEL(lastResult)

		vkGetBufferMemoryRequirements(device, buffer->staging_buffer, &lastRequirements);

	vlib_buffer_memory_allocate_info.allocationSize = lastRequirements.size;
	vlib_buffer_memory_allocate_info.memoryTypeIndex = vlib_device_host_visible_coherent_index;
	lastResult = vkAllocateMemory(device, &vlib_buffer_memory_allocate_info, nullptr, &buffer->staging_buffer_device_memory);
	HANDEL(lastResult)

		lastResult = vkBindBufferMemory(device, buffer->staging_buffer, buffer->staging_buffer_device_memory, 0);
	HANDEL(lastResult)

		staging_buffer.push_back(buffer);
}

void destroyStagingBuffer() {
	for each(StagingBuffer * buffer in staging_buffer) {
		vkFreeMemory(device, buffer->staging_buffer_device_memory, nullptr);
		vkDestroyBuffer(device, buffer->staging_buffer, nullptr);
	}
}
