#pragma once

#include "Device.cpp"

namespace Pipeline {

	struct Swapchain {
		uint32_t image_count;
		VkSharingMode sharing_mode;
		VkImageUsageFlags image_usage_flag;
		VkPresentModeKHR present_mode;
		VkSwapchainKHR* swapchain;
		vector<VkImageView> image_view_swapchain;
		Application app;
		Device dev;
	};


}