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
	Window* win = windowList[0];

	lastResult = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, win->surface, &surface_capabilities);
	CHECKFAIL;

		imagecount = TGE_MIN(TGE_MAX(imagecount, surface_capabilities.minImageCount), surface_capabilities.maxImageCount);

	VkSwapchainCreateInfoKHR swapchain_create_info = {
		VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		nullptr,
		0,
		win->surface,
		imagecount,
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
		CHECKFAIL;
	}

	lastResult = vkGetSwapchainImagesKHR(device, swapchain, &imagecount, nullptr);
	CHECKFAIL;

		swapchain_images.resize(imagecount);
	lastResult = vkGetSwapchainImagesKHR(device, swapchain, &imagecount, swapchain_images.data());
	CHECKFAIL;
}

void createColorResouce() {
	VkImageCreateInfo imageCreateInfo;
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.pNext = nullptr;
	imageCreateInfo.flags = 0;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.format = used_format.format;
	imageCreateInfo.extent.width = windowList[0]->width;
	imageCreateInfo.extent.height = windowList[0]->height;
	imageCreateInfo.extent.depth = 1;
	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.samples = usedMSAAFlag;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.queueFamilyIndexCount = 0;
	imageCreateInfo.pQueueFamilyIndices = nullptr;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	lastResult = vkCreateImage(device, &imageCreateInfo, nullptr, &color_image);
	CHECKFAIL;

	VkMemoryRequirements requierments;
	vkGetImageMemoryRequirements(device, depth_image, &requierments);

	vlibBufferMemoryAllocateInfo.allocationSize = requierments.size;
	vlibBufferMemoryAllocateInfo.memoryTypeIndex = vlibDeviceLocalMemoryIndex;
	lastResult = vkAllocateMemory(device, &vlibBufferMemoryAllocateInfo, nullptr, &color_image_memory);
	CHECKFAIL;

	lastResult = vkBindImageMemory(device, color_image, color_image_memory, 0);
	CHECKFAIL;

	VkImageViewCreateInfo imageViewCreateInfo;
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.pNext = nullptr;
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = color_image;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = used_format.format;
	imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;
	lastResult = vkCreateImageView(device, &imageViewCreateInfo, nullptr, &color_image_view);
	CHECKFAIL;
}

void destroyColorResouce() {
	vkDestroyImageView(device, color_image_view, nullptr);
	vkFreeMemory(device, color_image_memory, nullptr);
	vkDestroyImage(device, color_image, nullptr);
}

void recreateSwapchain(IndexBuffer* ibuffer, VertexBuffer* vbuffer) {
	lastResult = vkDeviceWaitIdle(device);
	CHECKFAIL;

	destroyFrameBuffer();
	vkFreeCommandBuffers(device, command_pool, (uint32_t)command_buffers.size(), command_buffers.data());
	destroyRenderPass();
	destroyColorResouce();
	destroyDepthTest();
	destroySwapchain();

	lastResult = vkDeviceWaitIdle(device);
	CHECKFAIL;

	lastResult = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, windowList[0]->surface, &surface_capabilities);
	CHECKFAIL;
	windowList[0]->width = surface_capabilities.currentExtent.width;
	windowList[0]->height = surface_capabilities.currentExtent.height;

	createColorResouce();
	createDepthTest();
	createRenderpass();
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

	tge::gmc::multiplier = (windowList[0]->height / (float)windowList[0]->width);

	tge::ui::ui_scene_entity.init();

	fillCommandBuffer(ibuffer, vbuffer);
}

void destroySwapchain() {
	vkDestroySwapchainKHR(device, swapchain, nullptr);
}