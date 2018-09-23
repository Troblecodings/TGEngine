#include "Swapchain.hpp"

VkSwapchainKHR swapchain;
VkSurfaceCapabilitiesKHR surface_capabilities;

void createSwapchain() {
	Window* win = window_list[0];

	last_result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(used_physical_device, win->surface, &surface_capabilities);
	HANDEL(last_result)

	image_count = math::u_min(math::u_max(image_count, surface_capabilities.minImageCount), surface_capabilities.maxImageCount);

	VkSwapchainCreateInfoKHR swapchain_create_info = {
		VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
	    nullptr,
	    0,
		win->surface,
	    image_count,
	    used_format.format,
		used_format.colorSpace,
		{
		    win->width,
		    win->height
		},
	    1,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
	    VK_SHARING_MODE_EXCLUSIVE,
	    0,
		nullptr,
		VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
	    used_present_mode,
	    VK_FALSE,
	    VK_NULL_HANDLE
	};

	last_result = vkCreateSwapchainKHR(device, &swapchain_create_info, nullptr, &swapchain);
	HANDEL(last_result)
}

void recreateSwapchain() {
	last_result = vkDeviceWaitIdle(device);
	HANDEL(last_result)

	destroyFrameBuffer();
	vkFreeCommandBuffers(device, command_pool, command_buffers.size(), command_buffers.data());
	destroyPipeline();
	destroyRenderPass();
	destroyDepthTest();
	destroySwapchain();

	last_result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(used_physical_device, window_list[0]->surface, &surface_capabilities);
	HANDEL(last_result)
	window_list[0]->width = surface_capabilities.currentExtent.width;
	window_list[0]->height = surface_capabilities.currentExtent.height;

	createDepthTest();
	createRenderpass();
	createPipeline();
	createSwapchain();
	createFramebuffer();
	createCommandBuffer();

	if (window_list[0]->height > window_list[0]->width) {
		fillUniformBuffer(&uniform_scale_buffer, (uint8_t*)&glm::vec2(1, (float)((float)window_list[0]->width / (float)window_list[0]->height)), sizeof(glm::vec2));
	}
	else if (window_list[0]->height < window_list[0]->width) {
		fillUniformBuffer(&uniform_scale_buffer, (uint8_t*)&glm::vec2((float)((float)window_list[0]->height / (float)window_list[0]->width), 1), sizeof(glm::vec2));
	}
	else {
		fillUniformBuffer(&uniform_scale_buffer, (uint8_t*)&glm::vec2(1, 1), sizeof(glm::vec2));
	}
}

void destroySwapchain() {
	vkDestroySwapchainKHR(device, swapchain, nullptr);
}