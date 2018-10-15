#include "DepthStencil.hpp"

VkImage depth_image;
VkImageView depth_image_view;
VkDeviceMemory depth_image_memory;

void createDepthTest()
{
	vlib_image_create_info.extent.width = window_list[0]->width;
	vlib_image_create_info.extent.height = window_list[0]->height;
	vlib_image_create_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	vlib_image_create_info.format = used_depth_format;
	last_result = vkCreateImage(device, &vlib_image_create_info, nullptr, &depth_image);
	HANDEL(last_result);

	VkMemoryRequirements requierments;
	vkGetImageMemoryRequirements(device, depth_image, &requierments);

	vlib_buffer_memory_allocate_info.allocationSize = requierments.size;
	vlib_buffer_memory_allocate_info.memoryTypeIndex = vlib_device_local_memory_index;
	last_result = vkAllocateMemory(device, &vlib_buffer_memory_allocate_info, nullptr, &depth_image_memory);
	HANDEL(last_result);

	last_result = vkBindImageMemory(device, depth_image, depth_image_memory, 0);
	HANDEL(last_result);

	vlib_image_view_create_info.format = used_depth_format;
	vlib_image_view_create_info.image = depth_image;
	vlib_image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	last_result = vkCreateImageView(device, &vlib_image_view_create_info, nullptr, &depth_image_view);
	HANDEL(last_result);
}

void destroyDepthTest()
{
	vkDestroyImageView(device, depth_image_view, nullptr);
	vkFreeMemory(device, depth_image_memory, nullptr);
	vkDestroyImage(device, depth_image, nullptr);
}
