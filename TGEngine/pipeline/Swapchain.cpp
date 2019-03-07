#include "Swapchain.hpp"

VkSwapchainKHR swapchain;
VkSurfaceCapabilitiesKHR surface_capabilities;
VkImage color_image;
VkImageView color_image_view;
VkDeviceMemory color_image_memory;

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
			(uint32_t)win->width,
			(uint32_t)win->height
		},
	    1,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
	    VK_SHARING_MODE_EXCLUSIVE,
	    0,
		nullptr,
		VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
	    used_present_mode,
	    VK_TRUE,
	    swapchain
	};

	last_result = vkCreateSwapchainKHR(device, &swapchain_create_info, nullptr, &swapchain);
	HANDEL(last_result)
}

void createColorResouce() {
	vlib_image_create_info.extent.width = window_list[0]->width;
	vlib_image_create_info.extent.height = window_list[0]->height;
	vlib_image_create_info.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	vlib_image_create_info.format = used_format.format;
	vlib_image_create_info.samples = used_msaa_flag;
	last_result = vkCreateImage(device, &vlib_image_create_info, nullptr, &color_image);
	HANDEL(last_result);

	VkMemoryRequirements requierments;
	vkGetImageMemoryRequirements(device, color_image, &requierments);

	vlib_buffer_memory_allocate_info.allocationSize = requierments.size;
	vlib_buffer_memory_allocate_info.memoryTypeIndex = vlib_device_local_memory_index;
	last_result = vkAllocateMemory(device, &vlib_buffer_memory_allocate_info, nullptr, &color_image_memory);
	HANDEL(last_result);

	last_result = vkBindImageMemory(device, color_image, color_image_memory, 0);
	HANDEL(last_result);

	vlib_image_view_create_info.format = used_format.format;
	vlib_image_view_create_info.image = color_image;
	vlib_image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	last_result = vkCreateImageView(device, &vlib_image_view_create_info, nullptr, &color_image_view);
	HANDEL(last_result);
}

void destroyColorResouce() {
	vkDestroyImageView(device, color_image_view, nullptr);
	vkFreeMemory(device, color_image_memory, nullptr);
	vkDestroyImage(device, color_image, nullptr);
}

void recreateSwapchain(IndexBuffer* ibuffer, VertexBuffer* vbuffer) {
	last_result = vkDeviceWaitIdle(device);
	HANDEL(last_result)

	destroyFrameBuffer();
	vkFreeCommandBuffers(device, command_pool, (uint32_t)command_buffers.size(), command_buffers.data());
	destroyPipeline();
	destroyRenderPass();
	destroyColorResouce();
	destroyDepthTest();
	destroySwapchain();

	last_result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(used_physical_device, window_list[0]->surface, &surface_capabilities);
	HANDEL(last_result)
	window_list[0]->width = surface_capabilities.currentExtent.width;
	window_list[0]->height = surface_capabilities.currentExtent.height;

	createColorResouce();
	createDepthTest();
	createRenderpass();
	vlib_rasterization_state.cullMode = VK_CULL_MODE_FRONT_BIT;
	createPipeline();
	vlib_rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
	createPipeline();
	createSwapchain();
	createFramebuffer();
	createCommandBuffer();

	multiplier = (window_list[0]->width / (float)window_list[0]->height);
	fillUniformBuffer(&ui_camera_uniform, &glm::ortho(-multiplier, multiplier, -1.0f, 1.0f), sizeof(glm::mat4));
	updateDescriptorSet(&ui_camera_uniform.descriptor, sizeof(glm::mat4));

	fillCommandBuffer(ibuffer, vbuffer);
}

void destroySwapchain() {
	vkDestroySwapchainKHR(device, swapchain, nullptr);
}