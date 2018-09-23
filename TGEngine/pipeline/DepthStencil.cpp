#include "DepthStencil.hpp"

VkImage depth_image;
VkImageView depth_image_view;
VkDeviceMemory depth_image_memory;

void createDepthTest()
{
	VkImageCreateInfo image_create_info = {
		VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		nullptr,
		0,
		VK_IMAGE_TYPE_2D,
		used_depth_format,
	{
		window_list[0]->width,
		window_list[0]->height,
		1
	},
		1,
		1,
		VK_SAMPLE_COUNT_1_BIT,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		0,
		nullptr,
		VK_IMAGE_LAYOUT_UNDEFINED
	};
	last_result = vkCreateImage(device, &image_create_info, nullptr, &depth_image);
	HANDEL(last_result);

	VkMemoryRequirements requierments;
	uint32_t index;
	for (index = 0; index < memory_properties.memoryTypeCount; index++) {
		if (memory_properties.memoryTypes[index].propertyFlags & (VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) break;
	}
	vkGetImageMemoryRequirements(device, depth_image, &requierments);

	VkMemoryAllocateInfo mem_alloc_info = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		nullptr,
		requierments.size,
		index
	};
	last_result = vkAllocateMemory(device, &mem_alloc_info, nullptr, &depth_image_memory);
	HANDEL(last_result);

	last_result = vkBindImageMemory(device, depth_image, depth_image_memory, 0);
	HANDEL(last_result);

	VkImageViewCreateInfo image_view_create_info = {
		VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		nullptr,
		0,
		depth_image,
		VK_IMAGE_VIEW_TYPE_2D,
		used_depth_format,
	{
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY
	},
	{
		VK_IMAGE_ASPECT_DEPTH_BIT,
		0,
		1,
		0,
		1
	}
	};
	last_result = vkCreateImageView(device, &image_view_create_info, nullptr, &depth_image_view);
	HANDEL(last_result);
}

void destroyDepthTest()
{
	vkDestroyImageView(device, depth_image_view, nullptr);
	vkFreeMemory(device, depth_image_memory, nullptr);
	vkDestroyImage(device, depth_image, nullptr);
}
