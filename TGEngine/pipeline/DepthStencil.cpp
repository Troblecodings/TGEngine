#include "DepthStencil.hpp"
#include "window/Window.hpp"

VkImage depth_image;
VkImageView depth_image_view;
VkDeviceMemory depth_image_memory;

void createDepthTest() {
	vlibImageCreateInfo.extent.width = windowList[0]->width;
	vlibImageCreateInfo.extent.height = windowList[0]->height;
	vlibImageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	vlibImageCreateInfo.format = used_depth_format;
	vlibImageCreateInfo.samples = usedMSAAFlag;
	vlibImageCreateInfo.mipLevels = 1;
	lastResult = vkCreateImage(device, &vlibImageCreateInfo, nullptr, &depth_image);
	HANDEL(lastResult);

	VkMemoryRequirements requierments;
	vkGetImageMemoryRequirements(device, depth_image, &requierments);

	vlibBufferMemoryAllocateInfo.allocationSize = requierments.size;
	vlibBufferMemoryAllocateInfo.memoryTypeIndex = vlibDeviceLocalMemoryIndex;
	lastResult = vkAllocateMemory(device, &vlibBufferMemoryAllocateInfo, nullptr, &depth_image_memory);
	HANDEL(lastResult);

	lastResult = vkBindImageMemory(device, depth_image, depth_image_memory, 0);
	HANDEL(lastResult);

	vlibImageViewCreateInfo.format = used_depth_format;
	vlibImageViewCreateInfo.image = depth_image;
	vlibImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	lastResult = vkCreateImageView(device, &vlibImageViewCreateInfo, nullptr, &depth_image_view);
	HANDEL(lastResult);
}

void destroyDepthTest() {
	vkDestroyImageView(device, depth_image_view, nullptr);
	vkFreeMemory(device, depth_image_memory, nullptr);
	vkDestroyImage(device, depth_image, nullptr);
}
