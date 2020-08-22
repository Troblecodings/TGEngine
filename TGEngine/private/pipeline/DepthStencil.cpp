#include "DepthStencil.hpp"
#include "window/Window.hpp"

VkImage depthImage;
VkImageView depthImageView;
VkDeviceMemory depthImageMemory;

void createDepthTest() {
	VkImageCreateInfo imageCreateInfo;
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.pNext = nullptr;
	imageCreateInfo.flags = 0;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.format = usedDepthFormat;
	imageCreateInfo.extent.width = tge::win::mainWindowWidth;
	imageCreateInfo.extent.height = tge::win::mainWindowHeight;
	imageCreateInfo.extent.depth = 1;
	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.samples = usedSampleFlag;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.queueFamilyIndexCount = 0;
	imageCreateInfo.pQueueFamilyIndices = nullptr;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	CHECKFAIL(vkCreateImage(device, &imageCreateInfo, nullptr, &depthImage));

	VkMemoryRequirements requierments;
	vkGetImageMemoryRequirements(device, depthImage, &requierments);

	VkMemoryAllocateInfo memoryAllocationInfo;
	memoryAllocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocationInfo.pNext = nullptr;
	memoryAllocationInfo.allocationSize = requierments.size;
	memoryAllocationInfo.memoryTypeIndex = vlibDeviceLocalMemoryIndex;
	CHECKFAIL(vkAllocateMemory(device, &memoryAllocationInfo, nullptr, &depthImageMemory));

	CHECKFAIL(vkBindImageMemory(device, depthImage, depthImageMemory, 0));

	VkImageViewCreateInfo imageViewCreateInfo;
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.pNext = nullptr;
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = depthImage;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = usedDepthFormat;
	imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;

	CHECKFAIL(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &depthImageView));
}

void destroyDepthTest() {
	vkDestroyImageView(device, depthImageView, nullptr);
	vkFreeMemory(device, depthImageMemory, nullptr);
	vkDestroyImage(device, depthImage, nullptr);
}
