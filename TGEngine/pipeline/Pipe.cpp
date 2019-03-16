#include "Pipe.hpp"

std::vector<VkPipeline> pipelines; 
std::vector<VkPipelineLayout> layouts;

void createPipeline(uint32_t layout) {
	Window* win = window_list[0];
	vlib_scissor.extent.height = (uint32_t) (vlib_viewport.height = (float) win->height);
	vlib_scissor.extent.width = (uint32_t) (vlib_viewport.width = (float) win->width);

	vlib_vertex_input_state.vertexAttributeDescriptionCount = (uint32_t)description_attributes.size();
	vlib_vertex_input_state.pVertexAttributeDescriptions = description_attributes.data();

	vlib_multisample_state.rasterizationSamples = used_msaa_flag;

	vlib_graphics_pipeline_create_info.stageCount = 2;
	vlib_graphics_pipeline_create_info.pStages = &shaders.data()[2];
	vlib_graphics_pipeline_create_info.layout = layouts[layout];
	vlib_graphics_pipeline_create_info.renderPass = render_pass;

	last_size = pipelines.size();
	pipelines.resize(last_size + 1);
	last_result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &vlib_graphics_pipeline_create_info, nullptr, &pipelines[last_size]);
	HANDEL(last_result);
}

uint32_t createPipelineLayout(uint32_t layout_count, VkDescriptorSetLayout* descriptor_set_layout) {
	last_size = layouts.size();
	layouts.resize(last_size + 1);

	vlib_layout_info.setLayoutCount = layout_count;
	vlib_layout_info.pSetLayouts = descriptor_set_layout;
	last_result = vkCreatePipelineLayout(device, &vlib_layout_info, nullptr, &layouts[last_size]);
	HANDEL(last_result);

	return last_size;
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
	layouts.clear();
	pipelines.clear();
}