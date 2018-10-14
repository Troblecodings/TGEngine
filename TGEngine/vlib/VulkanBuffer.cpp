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