#include "Pipe.hpp"

std::vector<VkPipeline> pipelines; 
std::vector<VkPipelineLayout> layouts;

void createPipeline() {
	TG_VECTOR_APPEND_NORMAL(layouts, VkPipelineLayout())

	Window* win = window_list[0];
	vlib_scissor.extent.height = (uint32_t) (vlib_viewport.height = (float) win->height);
	vlib_scissor.extent.width = (uint32_t) (vlib_viewport.width = (float) win->width);

	vlib_layout_info.setLayoutCount = (uint32_t) descriptor_set_layouts.size();
	vlib_layout_info.pSetLayouts = descriptor_set_layouts.data();
	last_result = vkCreatePipelineLayout(device, &vlib_layout_info, nullptr, &layouts[last_size]);

	vlib_graphics_pipeline_create_info.stageCount = (uint32_t)shaders.size();
	vlib_graphics_pipeline_create_info.pStages = shaders.data();
	vlib_graphics_pipeline_create_info.layout = layouts[last_size];
	vlib_graphics_pipeline_create_info.renderPass = render_pass;
	TG_VECTOR_APPEND_NORMAL(pipelines, VkPipeline())
	last_result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &vlib_graphics_pipeline_create_info, nullptr, &pipelines[last_size]);
	HANDEL(last_result);
}

void destroyPipeline() {
	for each (VkPipelineLayout var in layouts)
	{
		vkDestroyPipelineLayout(device, var, nullptr);
	}
	for each (VkPipeline var in pipelines)
	{
		vkDestroyPipeline(device, var, nullptr);
	}
}