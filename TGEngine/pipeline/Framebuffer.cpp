#include "Framebuffer.hpp"
#include "window/Window.hpp"
#include "RenderPass.hpp"
#include "Swapchain.hpp"
#include "PrePipeline.hpp"

VkFramebuffer* frameBuffer;
VkImageView* imageView;

void createFramebuffer() {
	frameBuffer = new VkFramebuffer[imagecount];
	imageView = new VkImageView[imagecount];

	VkFramebufferCreateInfo framebufferCreateInfo;
	framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferCreateInfo.pNext = nullptr;
	framebufferCreateInfo.flags = 0;
	framebufferCreateInfo.renderPass = renderpass;
	framebufferCreateInfo.attachmentCount = 3;
	framebufferCreateInfo.pAttachments = VK_NULL_HANDLE;
	framebufferCreateInfo.width = tge::win::mainWindowWidth;
	framebufferCreateInfo.height = tge::win::mainWindowHeight;
	framebufferCreateInfo.layers = 1;

	VkImageView views[3];
	views[0] = colorImageView;
	views[1] = depth_image_view;

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
		framebufferCreateInfo.pAttachments = views;

		CHECKFAIL(vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &frameBuffer[i]));
	}
}

void destroyFrameBuffer() {
	for (size_t i = 0; i < imagecount; i++) {
		vkDestroyFramebuffer(device, frameBuffer[i], nullptr);
		vkDestroyImageView(device, imageView[i], nullptr);
	}
}