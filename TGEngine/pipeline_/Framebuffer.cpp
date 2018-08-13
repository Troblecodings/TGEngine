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
	for (size_t i = 0; i < image_count; i++)
	{
	
		VkImageViewCreateInfo image_view_create_info = {
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		    nullptr,
			0,
		    image[i],
		    VK_IMAGE_VIEW_TYPE_2D,
			used_format.format,
			{
				VK_COMPONENT_SWIZZLE_IDENTITY,
				VK_COMPONENT_SWIZZLE_IDENTITY,
				VK_COMPONENT_SWIZZLE_IDENTITY,
				VK_COMPONENT_SWIZZLE_IDENTITY
            },
			{
				VK_IMAGE_ASPECT_COLOR_BIT,
				0,
				1,
				0,
				1,
            }
		};
		last_result = vkCreateImageView(device, &image_view_create_info, nullptr, &image_view[i]);
        HANDEL(last_result)

		

		VkFramebufferCreateInfo framebuffer_create_info = {
			VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		    nullptr,
		    0,
		    render_pass,
		    1,
		    &image_view[i],
			win->width,
			win->height,
		    1
		};
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