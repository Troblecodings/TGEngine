#include "../../public/vlib/VulkanCommandBuffer.hpp"

VkCommandBufferAllocateInfo vlibCommandBufferAllocateInfo = {
	VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
	nullptr,
	VK_NULL_HANDLE,
	VK_COMMAND_BUFFER_LEVEL_PRIMARY
};

VkCommandBufferBeginInfo vlibCommandBufferBeginInfo = {
	VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
	nullptr,
	VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
	nullptr
};

VkClearValue vlibClearValues[2] = {
	{
		{
			1,
			1,
			1,
			1
		}
	},
	{
		{
			1.0f,
			0
		}
	}
};
