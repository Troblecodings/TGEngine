#include "Framebuffer.hpp"
#include "window/Window.hpp"
#include "RenderPass.hpp"
#include "Swapchain.hpp"
#include "PrePipeline.hpp"

VkFramebuffer* framenBuffer;
VkImageView* imageView;

void createFramebuffer() {
	framenBuffer = new VkFramebuffer[imagecount];
	imageView = new VkImageView[imagecount];

	VkFramebufferCreateInfo frameBufferCreateInfo;
	frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	frameBufferCreateInfo.pNext = nullptr;
	frameBufferCreateInfo.flags = 0;
	frameBufferCreateInfo.renderPass = renderpass;
	frameBufferCreateInfo.attachmentCount = 3;
	frameBufferCreateInfo.pAttachments = VK_NULL_HANDLE;
	frameBufferCreateInfo.width = tge::win::mainWindowWidth;
	frameBufferCreateInfo.height = tge::win::mainWindowHeight;
	frameBufferCreateInfo.layers = 1;

	VkImageView views[3];
	views[0] = colorImageView;
	views[1] = depthImageView;

	VkImageViewCreateInfo imageViewCreateInfo;
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.pNext = nullptr;
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = VK_NULL_HANDLE;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = usedSurfaceFormat.format;
	// All channels are there identity therefore R = R, G = G, B = B, A = A => for color channel mapping
	imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;

	for (size_t i = 0; i < imagecount; i++) {
		imageViewCreateInfo.image = swapchainImages[i];
		CHECKFAIL(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &imageView[i]));

		views[2] = imageView[i];
		frameBufferCreateInfo.pAttachments = views;

		CHECKFAIL(vkCreateFramebuffer(device, &frameBufferCreateInfo, nullptr, &framenBuffer[i]));
	}
}

void destroyFrameBuffer() {
	for (size_t i = 0; i < imagecount; i++) {
		vkDestroyFramebuffer(device, framenBuffer[i], nullptr);
		vkDestroyImageView(device, imageView[i], nullptr);
	}
}