#include "Framebuffer.hpp"

std::vector<VkFramebuffer> frame_buffer;
std::vector<VkImageView> image_view;

void createFramebuffer() {
	Window* win = windowList[0];

	frame_buffer.resize(imagecount);
	image_view.resize(imagecount);

	vlibImageViewCreateInfo.format = used_format.format;
	vlibImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	vlibImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	vlibImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;

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

	for(size_t i = 0; i < imagecount; i++) {
		vlibImageViewCreateInfo.image = swapchain_images[i];
		lastResult = vkCreateImageView(device, &vlibImageViewCreateInfo, nullptr, &image_view[i]);
		HANDEL(lastResult)

			views[2] = image_view[i];
		framebuffer_create_info.pAttachments = views;

		lastResult = vkCreateFramebuffer(device, &framebuffer_create_info, nullptr, &frame_buffer[i]);
		HANDEL(lastResult)
	}
}

void destroyFrameBuffer() {
	for(size_t i = 0; i < imagecount; i++) {
		vkDestroyFramebuffer(device, frame_buffer[i], nullptr);
		vkDestroyImageView(device, image_view[i], nullptr);
	}
}