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
	CHECKFAIL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, tge::win::windowSurface, &surface_capabilities));

	imagecount = TGE_MIN(TGE_MAX(imagecount, surface_capabilities.minImageCount), surface_capabilities.maxImageCount);

	VkSwapchainCreateInfoKHR swapchain_create_info = {
		VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		nullptr,
		0,
		tge::win::windowSurface,
		imagecount,
		usedSurfaceFormat.format,
		usedSurfaceFormat.colorSpace,
		{
			tge::win::mainWindowWidth,
			tge::win::mainWindowHeight
		},
		1,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		0,
		nullptr,
		VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		usedPresentMode,
		VK_TRUE,
		swapchain
	};

	lastResult = vkCreateSwapchainKHR(device, &swapchain_create_info, nullptr, &swapchain);
	if (lastResult == VK_ERROR_INITIALIZATION_FAILED) {
		// TODO recover
	} else {
		CHECKFAIL(lastResult)
	}

	CHECKFAIL(vkGetSwapchainImagesKHR(device, swapchain, &imagecount, nullptr));

	swapchain_images.resize(imagecount);
	CHECKFAIL(vkGetSwapchainImagesKHR(device, swapchain, &imagecount, swapchain_images.data()));
}

void createColorResouce() {
	VkImageCreateInfo imageCreateInfo;
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.pNext = nullptr;
	imageCreateInfo.flags = 0;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.format = usedSurfaceFormat.format;
	imageCreateInfo.extent.width = tge::win::mainWindowWidth;
	imageCreateInfo.extent.height = tge::win::mainWindowHeight;

	imageCreateInfo.extent.depth = 1;
	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.samples = usedSampleFlag;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.queueFamilyIndexCount = 0;
	imageCreateInfo.pQueueFamilyIndices = nullptr;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	CHECKFAIL(vkCreateImage(device, &imageCreateInfo, nullptr, &color_image));

	VkMemoryRequirements requierments;
	vkGetImageMemoryRequirements(device, color_image, &requierments);

	vlibBufferMemoryAllocateInfo.allocationSize = requierments.size;
	vlibBufferMemoryAllocateInfo.memoryTypeIndex = vlibDeviceLocalMemoryIndex;
	CHECKFAIL(vkAllocateMemory(device, &vlibBufferMemoryAllocateInfo, nullptr, &color_image_memory));

	CHECKFAIL(vkBindImageMemory(device, color_image, color_image_memory, 0));

	VkImageViewCreateInfo imageViewCreateInfo;
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.pNext = nullptr;
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = color_image;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = usedSurfaceFormat.format;
	imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;
	CHECKFAIL(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &color_image_view));
}

void destroyColorResouce() {
	vkDestroyImageView(device, color_image_view, nullptr);
	vkFreeMemory(device, color_image_memory, nullptr);
	vkDestroyImage(device, color_image, nullptr);
}

void recreateSwapchain() {
	CHECKFAIL(vkDeviceWaitIdle(device));

	destroyFrameBuffer();
	vkFreeCommandBuffers(device, commandPool, (uint32_t)4, commandBuffer);
	destroyRenderPass();
	destroyColorResouce();
	destroyDepthTest();
	destroySwapchain();

	CHECKFAIL(vkDeviceWaitIdle(device));

	CHECKFAIL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, tge::win::windowSurface, &surface_capabilities));
	tge::win::mainWindowWidth = surface_capabilities.currentExtent.width;
	tge::win::mainWindowHeight = surface_capabilities.currentExtent.height;

	createColorResouce();
	createDepthTest();
	createRenderpass();
	createSwapchain();
	if (lastResult == VK_ERROR_INITIALIZATION_FAILED) {
		OUT_LV_DEBUG("Windows break the swapchain!")
			for each (VkImage var in swapchain_images) {
				vkDestroyImage(device, var, nullptr);
			}
		swapchain = VK_NULL_HANDLE;
		recreateSwapchain();
		return;
	}
	createFramebuffer();
	createCommandBuffer();

	tge::ui::ui_scene_entity.init();

	fillCommandBuffer();
}

void destroySwapchain() {
	vkDestroySwapchainKHR(device, swapchain, nullptr);
}