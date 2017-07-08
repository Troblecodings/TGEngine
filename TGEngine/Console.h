#pragma once

#define GLFW_INCLUDE_VULKAN
#include <thread>
#include "stdbase.h"

using namespace std;

namespace GLVV {
	VkDevice cdevice = {};
	VkSwapchainKHR SwapChain = {};
	vector<VkImageView> currentImage = {};
	VkShaderModule vermodule;
	VkShaderModule fragmodule;
	VkPipelineLayout pipline_layout;
	VkRenderPass render_pass;
	VkPipeline pipeline;
	vector<VkFramebuffer> framebuffers = {};
	VkCommandPool commandpool = {};
	vector<VkCommandBuffer> commandbuffers = {};
	VkSemaphore renderAvailable;
	VkSemaphore renderEnd;
	VkQueue queue;
	int actual_image_count = 0;
}