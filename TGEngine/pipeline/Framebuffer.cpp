#include "Framebuffer.hpp"

std::vector<VkFramebuffer> frame_buffer;
std::vector<VkImageView> image_view;
std::vector<VkImage> image;

void createFramebuffer() {
	Window* win = window_list[0];

	image.resize(image_count);
	vkGetSwapchainImagesKHR(device, swapchain, &image_count, image.data());

	frame_buffer.resize(image_count); 
	image_view.resize(image_count); 

	vlib_image_view_create_info.format = used_format.format;
	vlib_image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

	VkFramebufferCreateInfo framebuffer_create_info = {
	     VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
	     nullptr,
	     0,
	     render_pass,
	     2,
	     VK_NULL_HANDLE,
	     win->width,
	     win->height,
	     1
	};

	VkImageView views[2] = { nullptr, depth_image_view };

	for (size_t i = 0; i < image_count; i++)
	{
		vlib_image_view_create_info.image = image[i];
		last_result = vkCreateImageView(device, &vlib_image_view_create_info, nullptr, &image_view[i]);
		HANDEL(last_result)

		views[0] = image_view[i];
		framebuffer_create_info.pAttachments = views;

		last_result = vkCreateFramebuffer(device, &framebuffer_create_info, nullptr, &frame_buffer[i]);
		HANDEL(last_result)
	}
}

void destroyFrameBuffer() {
	for (size_t i = 0; i < image_count; i++)
	{
		vkDestroyFramebuffer(device, frame_buffer[i], nullptr);
		vkDestroyImageView(device, image_view[i], nullptr);
	}
}