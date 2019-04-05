#include "StagingBuffer.hpp"

std::vector<StagingBuffer*> staging_buffer;

void createStagingBuffer(StagingBuffer* buffer) {
	vlib_buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	vlib_buffer_create_info.size = buffer->size;
	last_result = vkCreateBuffer(device, &vlib_buffer_create_info, nullptr, &buffer->staging_buffer);
	HANDEL(last_result)

	vkGetBufferMemoryRequirements(device, buffer->staging_buffer, &last_requirements);

	vlib_buffer_memory_allocate_info.allocationSize = last_requirements.size;
	vlib_buffer_memory_allocate_info.memoryTypeIndex = vlib_device_host_visible_coherent_index;
	last_result = vkAllocateMemory(device, &vlib_buffer_memory_allocate_info, nullptr, &buffer->staging_buffer_device_memory);
	HANDEL(last_result)

	last_result = vkBindBufferMemory(device, buffer->staging_buffer, buffer->staging_buffer_device_memory, 0);
	HANDEL(last_result)

	staging_buffer.push_back(buffer);
}

void destroyStagingBuffer()
{
	for each(StagingBuffer* buffer in staging_buffer) {
		vkFreeMemory(device, buffer->staging_buffer_device_memory, nullptr);
		vkDestroyBuffer(device, buffer->staging_buffer, nullptr);
	}
}
