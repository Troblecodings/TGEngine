#include "VulkanBuffer.hpp"

VkBufferCreateInfo vlibBufferCreateInfo = {
	VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
	nullptr,
	0,
	0,
	VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	VK_SHARING_MODE_EXCLUSIVE,
	0,
	nullptr
};

VkMemoryAllocateInfo vlibBufferMemoryAllocateInfo = {
	VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
	nullptr,
	0,
	0
};

VkBufferCopy vlibBufferCopy = {
	0,
	0,
	0
};

uint32_t vlibDeviceLocalMemoryIndex;
uint32_t vlibDeviceHostVisibleCoherentIndex;