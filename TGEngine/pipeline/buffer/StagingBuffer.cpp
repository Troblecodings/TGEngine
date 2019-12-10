#include "StagingBuffer.hpp"

std::vector<StagingBuffer*> staging_buffer;

void createStagingBuffer(StagingBuffer* buffer) {
	vlibBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	vlibBufferCreateInfo.size = buffer->size;
	lastResult = vkCreateBuffer(device, &vlibBufferCreateInfo, nullptr, &buffer->staging_buffer);
	CHECKFAIL;

		VkMemoryRequirements lastRequirements;
		vkGetBufferMemoryRequirements(device, buffer->staging_buffer, &lastRequirements);

	vlibBufferMemoryAllocateInfo.allocationSize = lastRequirements.size;
	vlibBufferMemoryAllocateInfo.memoryTypeIndex = vlibDeviceHostVisibleCoherentIndex;
	lastResult = vkAllocateMemory(device, &vlibBufferMemoryAllocateInfo, nullptr, &buffer->staging_buffer_device_memory);
	CHECKFAIL;

		lastResult = vkBindBufferMemory(device, buffer->staging_buffer, buffer->staging_buffer_device_memory, 0);
	CHECKFAIL;

		staging_buffer.push_back(buffer);
}

void destroyStagingBuffer() {
	for each(StagingBuffer * buffer in staging_buffer) {
		vkFreeMemory(device, buffer->staging_buffer_device_memory, nullptr);
		vkDestroyBuffer(device, buffer->staging_buffer, nullptr);
	}
}
