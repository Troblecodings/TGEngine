#pragma once

#include <time.h>
#include <chrono>
#define GLFW_INCLUDE_VULKAN
#include <thread>
#include "Window.cpp"
#include "stdbase.h"

using namespace std;

void printTime() {
	time_t tim;
	time(&tim);
	tm point;
	gmtime_s(&point, &tim);
	cout << "[" << point.tm_hour << ":" << point.tm_min << ":" << point.tm_sec << "]:";
}




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