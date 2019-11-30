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
	HANDEL(lastResult)

		imagecount = TGE_MIN(TGE_MIN(imagecount, surface_capabilities.minImageCount), surface_capabilities.maxImageCount);

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
		HANDEL(lastResult)
	}

	lastResult = vkGetSwapchainImagesKHR(device, swapchain, &imagecount, nullptr);
	HANDEL(lastResult)

		swapchain_images.resize(imagecount);
	lastResult = vkGetSwapchainImagesKHR(device, swapchain, &imagecount, swapchain_images.data());
	HANDEL(lastResult)
}

void createColorResouce() {
	vlibImageCreateInfo.extent.width = windowList[0]->width;
	vlibImageCreateInfo.extent.height = windowList[0]->height;
	vlibImageCreateInfo.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	vlibImageCreateInfo.format = used_format.format;
	vlibImageCreateInfo.samples = usedMSAAFlag;
	vlibImageCreateInfo.mipLevels = 1;
	lastResult = vkCreateImage(device, &vlibImageCreateInfo, nullptr, &color_image);
	HANDEL(lastResult);

	VkMemoryRequirements requierments;
	vkGetImageMemoryRequirements(device, color_image, &requierments);

	vlibBufferMemoryAllocateInfo.allocationSize = requierments.size;
	vlibBufferMemoryAllocateInfo.memoryTypeIndex = vlibDeviceLocalMemoryIndex;
	lastResult = vkAllocateMemory(device, &vlibBufferMemoryAllocateInfo, nullptr, &color_image_memory);
	HANDEL(lastResult);

	lastResult = vkBindImageMemory(device, color_image, color_image_memory, 0);
	HANDEL(lastResult);

	vlibImageViewCreateInfo.format = used_format.format;
	vlibImageViewCreateInfo.image = color_image;
	vlibImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	lastResult = vkCreateImageView(device, &vlibImageViewCreateInfo, nullptr, &color_image_view);
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
	destroyRenderPass();
	destroyColorResouce();
	destroyDepthTest();
	destroySwapchain();

	lastResult = vkDeviceWaitIdle(device);
	HANDEL(lastResult)

		lastResult = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, windowList[0]->surface, &surface_capabilities);
	HANDEL(lastResult)
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