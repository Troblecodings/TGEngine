#include "Swapchain.hpp"
#include "window/Window.hpp"
#include "../gamecontent/Actor.hpp"
#include "../resources/ShaderPipes.hpp"

std::vector<VkImage> swapchain_images;
VkSwapchainKHR swapchain;
VkSurfaceCapabilitiesKHR surface_capabilities;
VkImage color_image;
VkImageView color_image_view;
VkDeviceMemory color_image_memory;

void createSwapchain() {
	Window* win = window_list[0];

	lastResult = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(used_physical_device, win->surface, &surface_capabilities);
	HANDEL(lastResult)

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

	lastResult = vkCreateSwapchainKHR(device, &swapchain_create_info, nullptr, &swapchain);
	if(lastResult != VK_ERROR_INITIALIZATION_FAILED) {
		HANDEL(lastResult)
	}

	lastResult = vkGetSwapchainImagesKHR(device, swapchain, &image_count, nullptr);
	HANDEL(lastResult)

		swapchain_images.resize(image_count);
	lastResult = vkGetSwapchainImagesKHR(device, swapchain, &image_count, swapchain_images.data());
	HANDEL(lastResult)
}

void createColorResouce() {
	vlib_image_create_info.extent.width = window_list[0]->width;
	vlib_image_create_info.extent.height = window_list[0]->height;
	vlib_image_create_info.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	vlib_image_create_info.format = used_format.format;
	vlib_image_create_info.samples = used_msaa_flag;
	vlib_image_create_info.mipLevels = 1;
	lastResult = vkCreateImage(device, &vlib_image_create_info, nullptr, &color_image);
	HANDEL(lastResult);

	VkMemoryRequirements requierments;
	vkGetImageMemoryRequirements(device, color_image, &requierments);

	vlib_buffer_memory_allocate_info.allocationSize = requierments.size;
	vlib_buffer_memory_allocate_info.memoryTypeIndex = vlib_device_local_memory_index;
	lastResult = vkAllocateMemory(device, &vlib_buffer_memory_allocate_info, nullptr, &color_image_memory);
	HANDEL(lastResult);

	lastResult = vkBindImageMemory(device, color_image, color_image_memory, 0);
	HANDEL(lastResult);

	vlib_image_view_create_info.format = used_format.format;
	vlib_image_view_create_info.image = color_image;
	vlib_image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	lastResult = vkCreateImageView(device, &vlib_image_view_create_info, nullptr, &color_image_view);
	HANDEL(lastResult);
}

void destroyColorResouce() {
	vkDestroyImageView(device, color_image_view, nullptr);
	vkFreeMemory(device, color_image_memory, nullptr);
	vkDestroyImage(device, color_image, nullptr);
}

void recreateSwapchain(IndexBuffer* ibuffer, VertexBuffer* vbuffer) {
	lastResult = vkDeviceWaitIdle(device);
	HANDEL(lastResult)

		destroyFrameBuffer();
	vkFreeCommandBuffers(device, command_pool, (uint32_t)command_buffers.size(), command_buffers.data());
	destroyPipeline();
	destroyRenderPass();
	destroyColorResouce();
	destroyDepthTest();
	destroySwapchain();

	lastResult = vkDeviceWaitIdle(device);
	HANDEL(lastResult)

		lastResult = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(used_physical_device, window_list[0]->surface, &surface_capabilities);
	HANDEL(lastResult)
		window_list[0]->width = surface_capabilities.currentExtent.width;
	window_list[0]->height = surface_capabilities.currentExtent.height;

	createColorResouce();
	createDepthTest();
	createRenderpass();
	for(size_t i = 0; i < tge::gmc::materiallist.size(); i++) {
		tge::gmc::materiallist[i]->createMaterial();
	}
	for (size_t i = 0; i < tge::gmc::actors.size(); i++) {
		for (size_t x = 0; x < tge::gmc::actors[i]->materials.size(); x++)
		{
			(&tge::gmc::actors[i]->materials[x])->createMaterial();
		}
	}
	createSwapchain();
	if(lastResult == VK_ERROR_INITIALIZATION_FAILED) {
		OUT_LV_DEBUG("Windows break the swapchain!")
			for each(VkImage var in swapchain_images) {
				vkDestroyImage(device, var, nullptr);
			}
		swapchain = VK_NULL_HANDLE;
		recreateSwapchain(ibuffer, vbuffer);
		return;
	}
	createFramebuffer();
	createCommandBuffer();

	tge::gmc::multiplier = (window_list[0]->height / (float)window_list[0]->width);

	tge::ui::ui_scene_entity.init();

	fillCommandBuffer(ibuffer, vbuffer);
}

void destroySwapchain() {
	vkDestroySwapchainKHR(device, swapchain, nullptr);
}