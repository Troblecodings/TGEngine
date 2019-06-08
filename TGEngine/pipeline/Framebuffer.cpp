#include "Framebuffer.hpp"

std::vector<VkFramebuffer> frame_buffer;
std::vector<VkImageView> image_view;

void createFramebuffer() {
	Window* win = window_list[0];

	frame_buffer.resize(image_count); 
	image_view.resize(image_count); 

	vlib_image_view_create_info.format = used_format.format;
	vlib_image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	vlib_image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	vlib_image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;

	VkFramebufferCreateInfo framebuffer_create_info = {
	     VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
	     nullptr,
	     0,
	     render_pass,
	     3,
	     VK_NULL_HANDLE,
		 (uint32_t)win->width,
		 (uint32_t)win->height,
	     1
	};

	VkImageView views[3] = { color_image_view, depth_image_view };

	for (size_t i = 0; i < image_count; i++)
	{
		vlib_image_view_create_info.image = swapchain_images[i];
		lastResult = vkCreateImageView(device, &vlib_image_view_create_info, nullptr, &image_view[i]);
		HANDEL(lastResult)

		views[2] = image_view[i];
		framebuffer_create_info.pAttachments = views;

		lastResult = vkCreateFramebuffer(device, &framebuffer_create_info, nullptr, &frame_buffer[i]);
		HANDEL(lastResult)
	}
}

void destroyFrameBuffer() {
	for (size_t i = 0; i < image_count; i++)
	{
		vkDestroyFramebuffer(device, frame_buffer[i], nullptr);
		vkDestroyImageView(device, image_view[i], nullptr);
	}
}