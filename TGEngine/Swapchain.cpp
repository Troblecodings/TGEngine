#pragma once

#include "Swapchain.h"

namespace Pipeline {

	void createSwapchain(Swapchain* chain) {

		VkSwapchainCreateInfoKHR swap_chain_creat_info = {};
		swap_chain_creat_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swap_chain_creat_info.surface = *chain->app->KHR;

		if (chain->device->khr_capabilities->minImageCount > chain->image_count) {
			swap_chain_creat_info.minImageCount = chain->device->khr_capabilities->minImageCount;
			chain->image_count = swap_chain_creat_info.minImageCount;
		}
		else if (chain->device->khr_capabilities->maxImageCount > chain->image_count) {
			swap_chain_creat_info.minImageCount = chain->image_count;
		}
		else {
			swap_chain_creat_info.minImageCount = chain->device->khr_capabilities->maxImageCount;
			chain->image_count = swap_chain_creat_info.minImageCount;
		}

		swap_chain_creat_info.imageFormat = chain->device->prefered_format;
		swap_chain_creat_info.imageColorSpace = chain->device->color_space;
		swap_chain_creat_info.imageExtent = chain->window->size;
		swap_chain_creat_info.imageSharingMode = chain->sharing_mode;
		swap_chain_creat_info.imageArrayLayers = 1;
		swap_chain_creat_info.imageUsage = chain->image_usage_flag;
		swap_chain_creat_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swap_chain_creat_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swap_chain_creat_info.presentMode = chain->present_mode;
		swap_chain_creat_info.clipped = VK_NULL_HANDLE;

		chain->swapchain = new VkSwapchainKHR;
		handel(vkCreateSwapchainKHR(*chain->device->device, &swap_chain_creat_info, nullptr, chain->swapchain));

		handel(vkGetSwapchainImagesKHR(*(*chain->device).device, *chain->swapchain, &chain->image_count, nullptr));

		vector<VkImage> image_array = {};
		image_array.resize(chain->image_count);
		handel(vkGetSwapchainImagesKHR(*(*chain->device).device, *chain->swapchain, &chain->image_count, image_array.data()));

		chain->image_view_swapchain.resize(chain->image_count);

		VkImageSubresourceRange range = {};
		range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		range.baseMipLevel = 0;
		range.levelCount = 1;
		range.baseArrayLayer = 0;
		range.layerCount = 1;

		for (size_t d = 0; d < chain->image_count; d++)
		{
			VkImageViewCreateInfo imview_create_info = {};
			imview_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imview_create_info.image = image_array[d];
			imview_create_info.format = (*chain->device).prefered_format;
			imview_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imview_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			imview_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			imview_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			imview_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			imview_create_info.subresourceRange = range;

			handel(vkCreateImageView(*(*chain->device).device, &imview_create_info, nullptr, &(chain->image_view_swapchain[d])));

		}
		chain->queue = new VkQueue;
		vkGetDeviceQueue(*(*chain->device).device, 0, 0, chain->queue);
	}

	void destroySwapchain(Swapchain* chain) {
		for (size_t i = 0; i < chain->image_count; i++)
		{
			vkDestroyImageView(*chain->device->device, chain->image_view_swapchain[i], nullptr);
		}
		vkDestroySwapchainKHR(*chain->device->device, *chain->swapchain, nullptr);
	}

}