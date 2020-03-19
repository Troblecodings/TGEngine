#include "Swapchain.hpp"
#include "window/Window.hpp"
#include "../gamecontent/Actor.hpp"
#include "../resources/ShaderPipes.hpp"
#include "../gamecontent/PlayerController.hpp"
#include "../pipeline/Pipe.hpp"
#include "../pipeline/Draw.hpp"

VkImage* swapchainImages;
VkSwapchainKHR swapchain;
VkSurfaceCapabilitiesKHR surfaceCapabilities;
VkImage colorImage;
VkImageView colorImageView;
VkDeviceMemory colorImageMemory;

void createSwapchain() {
	CHECKFAIL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, tge::win::windowSurface, &surfaceCapabilities));

	imageCount = TGE_MIN(TGE_MAX(imageCount, surfaceCapabilities.minImageCount), surfaceCapabilities.maxImageCount);

	VkSwapchainCreateInfoKHR swapchainCreateInfo;
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.pNext = nullptr;
	swapchainCreateInfo.flags = 0;
	swapchainCreateInfo.surface = tge::win::windowSurface;
	swapchainCreateInfo.minImageCount = imageCount;
	swapchainCreateInfo.imageFormat = usedSurfaceFormat.format;
	swapchainCreateInfo.imageColorSpace = usedSurfaceFormat.colorSpace;
	swapchainCreateInfo.imageExtent.width = tge::win::mainWindowWidth;
	swapchainCreateInfo.imageExtent.height = tge::win::mainWindowHeight;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.queueFamilyIndexCount = 0;
	swapchainCreateInfo.pQueueFamilyIndices = nullptr;
	swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.presentMode = usedPresentMode;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;


	lastResult = vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain);
	if (lastResult == VK_ERROR_INITIALIZATION_FAILED) {
		// TODO recover
	} else {
		CHECKFAIL(lastResult)
	}

	CHECKFAIL(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr));
	
	swapchainImages = new VkImage[imageCount];
	CHECKFAIL(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages));
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
	CHECKFAIL(vkCreateImage(device, &imageCreateInfo, nullptr, &colorImage));

	VkMemoryRequirements requirements;
	vkGetImageMemoryRequirements(device, colorImage, &requirements);

	VkMemoryAllocateInfo memoryAllocationInfo;
	memoryAllocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocationInfo.pNext = nullptr;
	memoryAllocationInfo.allocationSize = requirements.size;
	memoryAllocationInfo.memoryTypeIndex = vlibDeviceLocalMemoryIndex;
	CHECKFAIL(vkAllocateMemory(device, &memoryAllocationInfo, nullptr, &colorImageMemory));

	CHECKFAIL(vkBindImageMemory(device, colorImage, colorImageMemory, 0));

	VkImageViewCreateInfo imageViewCreateInfo;
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.pNext = nullptr;
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = colorImage;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = usedSurfaceFormat.format;
	imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;
	CHECKFAIL(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &colorImageView));
}

void destroyColorResouce() {
	vkDestroyImageView(device, colorImageView, nullptr);
	vkFreeMemory(device, colorImageMemory, nullptr);
	vkDestroyImage(device, colorImage, nullptr);
}

void recreateSwapchain() {
	CHECKFAIL(vkDeviceWaitIdle(device));

	destroyFrameBuffer();
	vkFreeCommandBuffers(device, commandPool, (uint32_t)4, commandBuffer);
	destroyRenderPass();
	destroyColorResouce();
	destroyDepthTest();
	destroySwapchain();
	tge::pip::destroyPipelines();

	CHECKFAIL(vkDeviceWaitIdle(device));

	CHECKFAIL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, tge::win::windowSurface, &surfaceCapabilities));
	tge::win::mainWindowWidth = surfaceCapabilities.currentExtent.width;
	tge::win::mainWindowHeight = surfaceCapabilities.currentExtent.height;

	createColorResouce();
	createDepthTest();
	createRenderpass();
	createSwapchain();
	tge::pip::initPipelines();
	createFramebuffer();
	createCommandBuffer();

	fillCommandBuffer();
	tge::gmc::playercontroller({});
}

void destroySwapchain() {
	vkDestroySwapchainKHR(device, swapchain, nullptr);
}