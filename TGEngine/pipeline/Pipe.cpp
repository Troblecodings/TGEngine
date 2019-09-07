#include "Pipe.hpp"
#include "window/Window.hpp"
#include "Descriptors.hpp"

std::vector<VkPipeline> pipelines;

uint32_t createPipeline(uint32_t layout) {
	Window* win = windowList[0];
	vlibScissor.extent.height = (uint32_t)(vlibViewport.height = (float)win->height);
	vlibScissor.extent.width = (uint32_t)(vlibViewport.width = (float)win->width);

	vlibMultisampleState.rasterizationSamples = used_msaa_flag;

	vlibGraphicsPipelineCreateInfo.layout = pipeLayouts[layout];
	vlibGraphicsPipelineCreateInfo.renderPass = render_pass;

	TG_VECTOR_GET_SIZE_AND_RESIZE(pipelines)
		lastResult = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &vlibGraphicsPipelineCreateInfo, nullptr, &pipelines[lastSize]);
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