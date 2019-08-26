#include "Pipe.hpp"
#include "window/Window.hpp"
#include "Descriptors.hpp"

std::vector<VkPipeline> pipelines;

uint32_t createPipeline(uint32_t layout) {
	Window* win = window_list[0];
	vlib_scissor.extent.height = (uint32_t)(vlib_viewport.height = (float)win->height);
	vlib_scissor.extent.width = (uint32_t)(vlib_viewport.width = (float)win->width);

	vlib_multisample_state.rasterizationSamples = used_msaa_flag;

	vlib_graphics_pipeline_create_info.layout = pipeLayouts[layout];
	vlib_graphics_pipeline_create_info.renderPass = render_pass;

	TG_VECTOR_GET_SIZE_AND_RESIZE(pipelines)
		lastResult = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &vlib_graphics_pipeline_create_info, nullptr, &pipelines[lastSize]);
	HANDEL(lastResult);
	return (uint32_t)lastSize;
}

void destroyPipelineLayout(uint32_t layout) {
	vkDestroyPipelineLayout(device, pipeLayouts[layout], nullptr);
}

void destroyPipeline(uint32_t layout) {
	vkDestroyPipeline(device, pipelines[layout], nullptr);
}

void destroyPipeline() {
	for each(VkPipeline var in pipelines) {
		vkDestroyPipeline(device, var, nullptr);
	}
	pipelines.clear();
}