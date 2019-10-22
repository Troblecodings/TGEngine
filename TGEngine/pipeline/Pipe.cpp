#include "Pipe.hpp"
#include "window/Window.hpp"
#include "Descriptors.hpp"

namespace tge::pip {

	void createPipelines(PipelineInputInfo* input, uint32_t size, VkPipeline* pipelines) {

		VkGraphicsPipelineCreateInfo* graphicsPipelineCreateInfo = new VkGraphicsPipelineCreateInfo[size];

		for (size_t i = 0; i < size; i++) {
			VkVertexInputBindingDescription vertexInputBindingDescriptor;
			vertexInputBindingDescriptor.binding = 0;
			vertexInputBindingDescriptor.stride = input[i].stride;
			vertexInputBindingDescriptor.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
			pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			pipelineVertexInputStateCreateInfo.pNext = nullptr;
			pipelineVertexInputStateCreateInfo.flags = 0;
			pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
			pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = &vertexInputBindingDescriptor;
			pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = input[i].pipe.inputCount;
			pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = input[i].pipe.inputs;

			graphicsPipelineCreateInfo[i].sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			graphicsPipelineCreateInfo[i].pNext = nullptr;
			graphicsPipelineCreateInfo[i].flags = 0;
			graphicsPipelineCreateInfo[i].stageCount = input[i].pipe.shaderCount;
			graphicsPipelineCreateInfo[i].pStages = input[i].pipe.shader;
			graphicsPipelineCreateInfo[i].pVertexInputState = &pipelineVertexInputStateCreateInfo;
			graphicsPipelineCreateInfo[i].pInputAssemblyState;
			graphicsPipelineCreateInfo[i].pTessellationState;
			graphicsPipelineCreateInfo[i].pViewportState;
			graphicsPipelineCreateInfo[i].pRasterizationState;
			graphicsPipelineCreateInfo[i].pMultisampleState;
			graphicsPipelineCreateInfo[i].pDepthStencilState;
			graphicsPipelineCreateInfo[i].pColorBlendState;
			graphicsPipelineCreateInfo[i].pDynamicState = nullptr;
			graphicsPipelineCreateInfo[i].layout = pipelineLayout;
			graphicsPipelineCreateInfo[i].renderPass = render_pass;
			graphicsPipelineCreateInfo[i].subpass = 0;
			graphicsPipelineCreateInfo[i].basePipelineHandle = nullptr;
			graphicsPipelineCreateInfo[i].basePipelineIndex = nullptr;
		}
		
		lastResult = vkCreateGraphicsPipelines(device, nullptr, size, graphicsPipelineCreateInfo, nullptr, pipelines);
		CHECKFAIL;
	}
}