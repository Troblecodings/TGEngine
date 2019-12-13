#include "DepthStencil.hpp"
#include "window/Window.hpp"

VkImage depth_image;
VkImageView depth_image_view;
VkDeviceMemory depth_image_memory;

void createDepthTest() {
	VkImageCreateInfo imageCreateInfo;
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.pNext = nullptr;
	imageCreateInfo.flags = 0;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.format = used_depth_format;
	imageCreateInfo.extent.width = windowList[0]->width;
	imageCreateInfo.extent.height = windowList[0]->height;
	imageCreateInfo.extent.depth = 1;
	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.samples = usedMSAAFlag;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.queueFamilyIndexCount = 0;
	imageCreateInfo.pQueueFamilyIndices = nullptr;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	CHECKFAIL(vkCreateImage(device, &imageCreateInfo, nullptr, &depth_image));

	VkMemoryRequirements requierments;
	vkGetImageMemoryRequirements(device, depth_image, &requierments);

	vlibBufferMemoryAllocateInfo.allocationSize = requierments.size;
	vlibBufferMemoryAllocateInfo.memoryTypeIndex = vlibDeviceLocalMemoryIndex;
	CHECKFAIL(vkAllocateMemory(device, &vlibBufferMemoryAllocateInfo, nullptr, &depth_image_memory));

	CHECKFAIL(vkBindImageMemory(device, depth_image, depth_image_memory, 0));

	VkImageViewCreateInfo imageViewCreateInfo;
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.pNext = nullptr;
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = depth_image;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = used_depth_format;
	imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;

	CHECKFAIL(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &depth_image_view));
}

void destroyDepthTest() {
	vkDestroyImageView(device, depth_image_view, nullptr);
	vkFreeMemory(device, depth_image_memory, nullptr);
	vkDestroyImage(device, depth_image, nullptr);
}
