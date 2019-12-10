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
	imageViewCreateInfo.subresourceRange;

	for(size_t i = 0; i < imagecount; i++) {
		imageViewCreateInfo.image = swapchain_images[i];
		lastResult = vkCreateImageView(device, &imageViewCreateInfo, nullptr, &image_view[i]);
		CHECKFAIL;

		views[2] = image_view[i];
		framebuffer_create_info.pAttachments = views;

		lastResult = vkCreateFramebuffer(device, &framebuffer_create_info, nullptr, &frame_buffer[i]);
		CHECKFAIL;
	}
}

void destroyFrameBuffer() {
	for(size_t i = 0; i < imagecount; i++) {
		vkDestroyFramebuffer(device, frame_buffer[i], nullptr);
		vkDestroyImageView(device, image_view[i], nullptr);
	}
}