#include "../../public/vlib/VulkanPipeline.hpp"

//Viewport and scissor
VkViewport vlibViewport = {
	0.0F,
	0.0F,
	0,
	0,
	0,
	1.0F
};

VkRect2D vlibScissor = {
	{
		0,
		0
	},
	{
		0,
		0
	}
};

VkPipelineViewportStateCreateInfo vlibViewportState = {
	VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
	nullptr,
	0,
	1,
	&vlibViewport,
	1,
	&vlibScissor
};


// Vertex binding
VkVertexInputBindingDescription vlibVertexBufferBinding = {
	0,
	0,
	VK_VERTEX_INPUT_RATE_VERTEX
};

VkPipelineVertexInputStateCreateInfo vlibVertexInputState = {
	VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
	nullptr,
	0,
	1,
	&vlibVertexBufferBinding,
	0,
	VK_NULL_HANDLE
};

//Assembly state
VkPipelineInputAssemblyStateCreateInfo vlibInputAssemblyState = {
	VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
	nullptr,
	0,
	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	VK_FALSE
};

//Tessellation
VkPipelineTessellationStateCreateInfo vlibTessellationState = {
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
VkPipelineMultisampleStateCreateInfo vlibMultisampleState = {
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
VkPipelineColorBlendAttachmentState vlibColorBlendAttachmentState = {
	VK_TRUE,
	VK_BLEND_FACTOR_SRC_ALPHA,
	VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
	VK_BLEND_OP_ADD,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_FACTOR_ZERO,
	VK_BLEND_OP_ADD,
	VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
};

VkPipelineColorBlendStateCreateInfo vlibColorBlendState = {
	VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
	nullptr,
	0,
	VK_FALSE,
	VK_LOGIC_OP_CLEAR,
	1,
	&vlibColorBlendAttachmentState,
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

//Pipeline
VkGraphicsPipelineCreateInfo vlibGraphicsPipelineCreateInfo = {
	VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
	nullptr,
	0,
	0,
	nullptr,
	&vlibVertexInputState,
	&vlibInputAssemblyState,
	&vlibTessellationState,
	&vlibViewportState,
	&vlibRasterizationState,
	&vlibMultisampleState,
	&vlibDepthStencilState,
	&vlibColorBlendState,
	nullptr,
	VK_NULL_HANDLE,
	VK_NULL_HANDLE,
	0,
	VK_NULL_HANDLE,
	0
};

