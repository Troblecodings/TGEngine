#pragma once

#include "stdbase.h"
#include "Window.h"
#include "RenderPass.h"

namespace Pipeline {

	struct Pipe
	{
		std::vector<VkPipelineShaderStageCreateInfo> shader;
		std::vector<VkFramebuffer> frame_buffer;
		std::vector<VkCommandBuffer> command_buffer;
		std::vector<VkImageView> image_views;
		VkCommandPool* command_pool;
		Window* window;
		VkPipeline* pipeline;
		VkDevice* device;
		RenderPass* render_pass;
		uint32_t image_count;
		VkSemaphore* available;
		VkSemaphore* end;
	};

	/*
	 * Creates a Pipeline
	 */
	void createPipeline(Pipe pipeline);

	/*
	 * Destroys the Pipeline
	 */
	void destroyPipeline(Pipe pipeline);
}