#include "VulkanBuffer.hpp"

VkBufferCreateInfo vlib_buffer_create_info = {
	VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
	nullptr,
	0,
	0,
	VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	VK_SHARING_MODE_EXCLUSIVE,
	0,
	nullptr
};

VkMemoryAllocateInfo vlib_buffer_memory_allocate_info = {
	VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
	nullptr,
	0,
	0
};

VkBufferCopy vlib_buffer_copy = {
	0,
	0,
	0
};

uint32_t vlib_device_local_memory_index;
uint32_t vlib_device_host_visible_coherent_index;