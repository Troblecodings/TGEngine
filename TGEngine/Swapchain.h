#pragma once

#include "stdbase.h"
#include "Device.h"
#include "Window.h"

namespace Pipeline {

	struct Swapchain {
		uint32_t image_count;
		VkSharingMode sharing_mode;
		VkImageUsageFlags image_usage_flag;
		VkSwapchainKHR* swapchain;
		vector<VkImageView> image_view_swapchain;
		Application* app;
		Device* device;
		Window* window;
		VkQueue* queue;
	};

	/*
	 * Creates a Swapchain
	 */
	SINCE(0, 0, 6)
	void createSwapchain(Swapchain* chain);

	/*
	 * Destroys the Swapchain
	 */
	SINCE(0, 0, 6)
	void destroySwapchain(Swapchain* chain);

}