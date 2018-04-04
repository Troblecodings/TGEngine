#include "Pipe.hpp"

VkPipeline pipeline;
VkPipelineLayout layout;
VkViewport viewport;
VkRect2D scissor;

void createPipeline() {

    //Viewport and so on
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

	VkPipelineViewportStateCreateInfo pViewportState = {
		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		nullptr,
		0,
		1,
		&viewport,
		1,
		&scissor
	};

	//Pipeline Layout
	VkPipelineLayoutCreateInfo layout_info = {
		VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		nullptr,
		0,
		1,
		&desc_layout,
		0,
		nullptr
	};
	last_result = vkCreatePipelineLayout(device, &layout_info, nullptr, &layout);
	HANDEL(last_result);

	// Vertex binding
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

	//Assembly state
	VkPipelineInputAssemblyStateCreateInfo pInputAssemblyState = {
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
	    nullptr,
	    0,
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	    VK_FALSE
	};

	//Tessellation
	VkPipelineTessellationStateCreateInfo pTessellationState = {
		VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
	    nullptr,
	    0,
	    1
	}; 

	//Rasterization
	VkPipelineRasterizationStateCreateInfo pRasterizationState = {
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
	    nullptr,
	    0,
	    VK_FALSE,
	    VK_FALSE,
	    VK_POLYGON_MODE_FILL,
		VK_CULL_MODE_BACK_BIT,
		VK_FRONT_FACE_CLOCKWISE,
	    VK_FALSE,
	    1,
	    0,
	    1,
	    1
	};

	//Multisample
	VkPipelineMultisampleStateCreateInfo pMultisampleState = {
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
	    nullptr,
	    0,
	    VK_SAMPLE_COUNT_1_BIT,
	    VK_FALSE,
	    1.0F,
	    nullptr,
	    VK_FALSE,
	    VK_FALSE
	};

	//Color blend
	VkPipelineColorBlendAttachmentState color_blend_state = {
		VK_TRUE,
		VK_BLEND_FACTOR_ONE,
		VK_BLEND_FACTOR_ZERO,
		VK_BLEND_OP_ADD,
		VK_BLEND_FACTOR_ONE,
		VK_BLEND_FACTOR_ZERO,
		VK_BLEND_OP_ADD,
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
	};

	VkPipelineColorBlendStateCreateInfo pColorBlendState = {
		VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
	    nullptr,
	    0,
	    VK_FALSE,
		VK_LOGIC_OP_NO_OP,
	    1,
		&color_blend_state,
		{VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY}
	};

	//Pipeline
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
	    nullptr,
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