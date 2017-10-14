#include "Pipe.hpp"

VkPipeline pipeline;
VkPipelineLayout layout;
VkViewport viewport;
VkRect2D scissor;

void createPipeline() {
	viewport = {
		0.0F,
		0.0F,
		static_cast<float>(width),
		static_cast<float>(height),
		0.0F,
		1.0F
	};

	scissor = {
		{
			0,
			0
        },
	    {
			width,
			height
	    }
	};

	VkPipelineLayoutCreateInfo layout_info = {
		VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		nullptr,
		0,
		0,
		nullptr,
		0,
		nullptr
	};
	last_result = vkCreatePipelineLayout(device, &layout_info, nullptr, &layout);
	HANDEL(last_result);

	VkVertexInputBindingDescription vertex_buffer_binding = {
		0,
		sizeof(Vertex),
		VK_VERTEX_INPUT_RATE_VERTEX
	};

	VkPipelineVertexInputStateCreateInfo pVertexInputState = {
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		nullptr,
		0,
		1,
		&vertex_buffer_binding,
		description_attributes.size(),
		description_attributes.data()
	};

	VkPipelineInputAssemblyStateCreateInfo pInputAssemblyState = {
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
	    nullptr,
	    0,
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	    VK_FALSE
	};

	VkPipelineTessellationStateCreateInfo pTessellationState = {
		VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
	    nullptr,
	    0,
	    1
	}; 

	VkPipelineViewportStateCreateInfo pViewportState = {
		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
	    nullptr,
	    0,
	    1,
	    &viewport,
	    1,
	    &scissor
	};

	VkPipelineRasterizationStateCreateInfo pRasterizationState = {
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
	    nullptr,
	    0,
	    VK_FALSE,
	    VK_FALSE,
	    VK_POLYGON_MODE_FILL,
		VK_CULL_MODE_FRONT_AND_BACK,
		VK_FRONT_FACE_CLOCKWISE,
	    VK_FALSE,
	    0,
	    0,
	    0,
	    1
	};

	VkPipelineMultisampleStateCreateInfo pMultisampleState = {
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
	    nullptr,
	    0,
	    VK_SAMPLE_COUNT_1_BIT,
	    VK_FALSE,
	    0,
	    0,
	    VK_FALSE,
	    VK_FALSE
	};

	VkPipelineDepthStencilStateCreateInfo pDepthStencilState = {
		VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
	    nullptr,
	    0,
	    VK_FALSE,
	    VK_FALSE,
	    VK_COMPARE_OP_NEVER,
	    VK_FALSE,
	    VK_FALSE,
		{},
		{},
	    0.0F,
	    1.0F
	};

	VkPipelineColorBlendStateCreateInfo pColorBlendState = {
		VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
	    nullptr,
	    0,
	    VK_FALSE,
		VK_LOGIC_OP_NO_OP,
	    0,
	    nullptr,
		{VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY}
	};

	VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = {
		VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
	    nullptr,
	    0,
	    shaders.size(),
	    shaders.data(),
		&pVertexInputState,
	    &pInputAssemblyState,
		&pTessellationState,
	    &pViewportState,
	    &pRasterizationState,
	    &pMultisampleState,
	    &pDepthStencilState,
	    &pColorBlendState,
	    nullptr,
	    layout,
	    render_pass,
	    0,
	    VK_NULL_HANDLE,
	    0
	};
	last_result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &graphics_pipeline_create_info, nullptr, &pipeline);
	HANDEL(last_result);
}

void destroyPipeline() {
	vkDestroyPipelineLayout(device, layout, nullptr);
	vkDestroyPipeline(device, pipeline, nullptr);
}