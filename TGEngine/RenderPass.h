#pragma once

#include "stdbase.h"
#include "Window.h"
#include "Debug.h"
#include "Device.h"

namespace Pipeline {

	struct RenderPass
	{
		Window* window;
		Device* device;
		VkPipelineLayout* layout;
		VkRenderPass* render_pass;
	};

	/*
	 * Creates a RenderPass and a PipelineLayout
	 */
	void createRenderPass(RenderPass* render_pass);

	/*
	 * Destroys the RenderPass and the PipelineLayout
	 */
	void destroyRenderPass(RenderPass* render_pass);

}