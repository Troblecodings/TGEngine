#include "Pipe.hpp"

VkPipeline pipeline;

void createPipeline() {
	VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = {
		VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
	    nullptr,
	    0,
	    ,
	pStages,
	pVertexInputState,
	pInputAssemblyState,
	pTessellationState,
	pViewportState,
	pRasterizationState,
	pMultisampleState,
	pDepthStencilState,
	pColorBlendState,
	pDynamicState,
	layout,
	    render_pass,
	    0,
	    VK_NULL_HANDLE,
	    0
	};

	vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &graphics_pipeline_create_info, nullptr, &pipeline);
}