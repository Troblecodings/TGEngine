#pragma once

#include "stdbase.h"
#include "Window.h"
#include "RenderPass.h"
#include "Swapchain.h"

namespace Pipeline {

	struct Pipe
	{
		std::vector<VkPipelineShaderStageCreateInfo>* shader;
		std::vector<VkFramebuffer>* frame_buffer;
		std::vector<VkCommandBuffer>* command_buffer;
		VkCommandPool* command_pool;
		Window* window;
		VkPipeline* pipeline;
		VkDevice* device;
		RenderPass* render_pass;
		VkSemaphore* available;
		VkSemaphore* end;
		Swapchain* swapchain;
	};

	/*
	 * Creates a Pipeline
	 */
	void createPipeline(Pipe* pipeline);

	/*
	 * Destroys the Pipeline
	 */
	void destroyPipeline(Pipe* pipeline);
}