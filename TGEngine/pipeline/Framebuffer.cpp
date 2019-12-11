#include "Framebuffer.hpp"

std::vector<VkFramebuffer> frame_buffer;
std::vector<VkImageView> image_view;

void createFramebuffer() {
	Window* win = windowList[0];

	frame_buffer.resize(imagecount);
	image_view.resize(imagecount);

	VkFramebufferCreateInfo framebuffer_create_info = {
		 VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		 nullptr,
		 0,
		 renderpass,
		 3,
		 VK_NULL_HANDLE,
		 (uint32_t)win->width,
		 (uint32_t)win->height,
		 1
	};

	VkImageView views[3];
	views[0] = color_image_view;
	views[1] = depth_image_view;

	VkImageViewCreateInfo imageViewCreateInfo;
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.pNext = nullptr;
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = VK_NULL_HANDLE;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = used_format.format;
	// All channels are there identity therefore R = R, G = G, B = B, A = A => for color channel mapping
	imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;

	for (size_t i = 0; i < imagecount; i++) {
		imageViewCreateInfo.image = swapchain_images[i];
		CHECKFAIL(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &image_view[i]));

		views[2] = image_view[i];
		framebuffer_create_info.pAttachments = views;

		CHECKFAIL(vkCreateFramebuffer(device, &framebuffer_create_info, nullptr, &frame_buffer[i]));
	}
}

void destroyFrameBuffer() {
	for (size_t i = 0; i < imagecount; i++) {
		vkDestroyFramebuffer(device, frame_buffer[i], nullptr);
		vkDestroyImageView(device, image_view[i], nullptr);
	}
}