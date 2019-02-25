#include "VulkanCommandBuffer.hpp"

VkCommandBufferAllocateInfo vlib_command_buffer_allocate_info = {
	VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
	nullptr,
	VK_NULL_HANDLE,
	VK_COMMAND_BUFFER_LEVEL_PRIMARY
};

VkCommandBufferBeginInfo vlib_command_buffer_begin_info = {
	VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
	nullptr,
	VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
	nullptr
};