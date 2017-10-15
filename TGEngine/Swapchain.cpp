#include "Swapchain.hpp"

VkSwapchainKHR swapchain;

void createSwapchain() {
	VkSwapchainCreateInfoKHR swapchain_create_info = {
		VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
	    nullptr,
	    0,
	    surface,
	    image_count,
	    VK_FORMAT_B8G8R8A8_UNORM,
		VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
		{
			width,
			height
		},
	    1,
	    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
	    VK_SHARING_MODE_EXCLUSIVE,
	    1,
		{ &queue_index },
		VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
	    VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
	    VK_PRESENT_MODE_MAILBOX_KHR,
	    VK_FALSE,
	    VK_NULL_HANDLE
	};

	last_result = vkCreateSwapchainKHR(device, &swapchain_create_info, nullptr, &swapchain);
	HANDEL(last_result)
}

void destroySwapchain() {
	vkDestroySwapchainKHR(device, swapchain, nullptr);
}