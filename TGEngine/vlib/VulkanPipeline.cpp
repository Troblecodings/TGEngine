#include "VulkanPipeline.hpp"

//Viewport and scissor
VkViewport vlib_viewport = {
	0.0F,
	0.0F,
	0,
	0,
	0,
	1.0F
};

VkRect2D vlib_scissor = {
	{
		0,
		0
	},
	{
		0,
		0
	}
};

VkPipelineViewportStateCreateInfo vlib_viewport_state = {
	VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
	nullptr,
	0,
	1,
	&vlib_viewport,
	1,
	&vlib_scissor
};


// Vertex binding
VkVertexInputBindingDescription vlib_vertex_buffer_binding = {
	0,
	VERTEX_SIZE,
	VK_VERTEX_INPUT_RATE_VERTEX
};

VkPipelineVertexInputStateCreateInfo vlib_vertex_input_state = {
	VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
	nullptr,
	0,
	1,
	&vlib_vertex_buffer_binding,
	0,
	VK_NULL_HANDLE
};

//Assembly state
VkPipelineInputAssemblyStateCreateInfo vlib_input_assembly_state = {
	VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
	nullptr,
	0,
	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	VK_FALSE
};

//Tessellation
VkPipelineTessellationStateCreateInfo vlib_tessellation_state = {
	VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
	nullptr,
	0,
	1
};

//Rasterization
VkPipelineRasterizationStateCreateInfo vlibRasterizationState = {
	VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
	nullptr,
	0,
	VK_TRUE,
	VK_FALSE,
	VK_POLYGON_MODE_FILL,
	VK_CULL_MODE_FRONT_BIT,
	VK_FRONT_FACE_CLOCKWISE,
	VK_FALSE,
	1,
	0,
	1,
	1
};

//Multisample
VkPipelineMultisampleStateCreateInfo vlib_multisample_state = {
	VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
	nullptr,
	0,
	VK_SAMPLE_COUNT_2_BIT,
	VK_FALSE,
	1.0F,
	nullptr,
	VK_FALSE,
	VK_FALSE
};

//Color blend
VkPipelineColorBlendAttachmentState vlib_color_blend_attachment_state = {
	VK_TRUE,
	VK_BLEND_FACTOR_SRC_ALPHA,
	VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
	VK_BLEND_OP_ADD,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_FACTOR_ZERO,
	VK_BLEND_OP_ADD,
	VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
};

VkPipelineColorBlendStateCreateInfo vlib_color_blend_state = {
	VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
	nullptr,
	0,
	VK_FALSE,
	VK_LOGIC_OP_CLEAR,
	1,
	&vlib_color_blend_attachment_state,
	{VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY}
};

VkPipelineDepthStencilStateCreateInfo vlibDepthStencilState = {
	VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
	nullptr,
	0,
	VK_TRUE,
	VK_TRUE,
	VK_COMPARE_OP_LESS,
	VK_FALSE,
	VK_FALSE,
{
},
{
},
0,
1.0f
};

//Pipeline Layout
VkPipelineLayoutCreateInfo vlib_layout_info = {
	VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
	nullptr,
	0,
	1,
	nullptr,
	0,
	nullptr
};

//Pipeline
VkGraphicsPipelineCreateInfo vlib_graphics_pipeline_create_info = {
	VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
	nullptr,
	0,
	0,
	nullptr,
	&vlib_vertex_input_state,
	&vlib_input_assembly_state,
	&vlib_tessellation_state,
	&vlib_viewport_state,
	&vlibRasterizationState,
	&vlib_multisample_state,
	&vlibDepthStencilState,
	&vlib_color_blend_state,
	nullptr,
	VK_NULL_HANDLE,
	VK_NULL_HANDLE,
	0,
	VK_NULL_HANDLE,
	0
};

