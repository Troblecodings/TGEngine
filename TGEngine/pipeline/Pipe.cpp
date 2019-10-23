#include "Pipe.hpp"
#include "RenderPass.hpp"
#include "Descriptors.hpp"
#include "PrePipeline.hpp"

namespace tge::pip {

	void createPipelines(PipelineInputInfo* input, uint32_t size, VkPipeline* pipelines) {

		VkGraphicsPipelineCreateInfo* graphicsPipelineCreateInfo = new VkGraphicsPipelineCreateInfo[size];

		for (size_t i = 0; i < size; i++) {
			PipelineInputInfo in = input[i];

			VkVertexInputBindingDescription vertexInputBindingDescriptor;
			vertexInputBindingDescriptor.binding = 0;
			vertexInputBindingDescriptor.stride = in.stride;
			vertexInputBindingDescriptor.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
			pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			pipelineVertexInputStateCreateInfo.pNext = nullptr;
			pipelineVertexInputStateCreateInfo.flags = 0;
			pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
			pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = &vertexInputBindingDescriptor;
			pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = in.pipe.inputCount;
			pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = in.pipe.inputs;

			VkPipelineInputAssemblyStateCreateInfo pipelineInputStateCreateInfo;
			pipelineInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			pipelineInputStateCreateInfo.pNext = nullptr;
			pipelineInputStateCreateInfo.flags = 0;
			pipelineInputStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			pipelineInputStateCreateInfo.primitiveRestartEnable = VK_FALSE;

			VkViewport viewport;
			viewport.x = 0;
			viewport.y = 0;
			viewport.width = in.width;
			viewport.height = in.height;
			viewport.minDepth = 0;
			viewport.maxDepth = 1.0;

			VkRect2D sicossor;
			sicossor.offset.x = 0;
			sicossor.offset.y = 0;
			sicossor.extent.width = viewport.width;
			sicossor.extent.height = viewport.height;

			VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo;
			pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			pipelineViewportStateCreateInfo.pNext = nullptr;
			pipelineViewportStateCreateInfo.flags = 0;
			pipelineViewportStateCreateInfo.viewportCount = 1;
			pipelineViewportStateCreateInfo.pViewports = &viewport;
			pipelineViewportStateCreateInfo.scissorCount = 1;
			pipelineViewportStateCreateInfo.pScissors = &sicossor;

			VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo;
			pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			pipelineRasterizationStateCreateInfo.pNext = nullptr;
			pipelineRasterizationStateCreateInfo.flags = 0;
			pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
			pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_TRUE;
			pipelineRasterizationStateCreateInfo.polygonMode = in.polygonMode;
			pipelineRasterizationStateCreateInfo.cullMode = in.cullMode;
			pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
			pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
			pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0;
			pipelineRasterizationStateCreateInfo.depthBiasClamp = 0;
			pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0;
			pipelineRasterizationStateCreateInfo.lineWidth = in.lineWidth;

			VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo;
			pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			pipelineMultisampleStateCreateInfo.pNext = nullptr;
			pipelineMultisampleStateCreateInfo.flags = 0;
			pipelineMultisampleStateCreateInfo.rasterizationSamples = usedMSAAFlag;
			pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
			pipelineMultisampleStateCreateInfo.minSampleShading = 0;
			pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
			pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
			pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

			VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState;
			pipelineColorBlendAttachmentState.blendEnable = VK_TRUE;
			pipelineColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
			pipelineColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
			pipelineColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
			pipelineColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			pipelineColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			pipelineColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
			pipelineColorBlendAttachmentState.colorWriteMask ;

			VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo;
			pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			pipelineColorBlendStateCreateInfo.pNext = nullptr;
			pipelineColorBlendStateCreateInfo.flags = 0;
			pipelineColorBlendStateCreateInfo.logicOpEnable = VK_TRUE;
			pipelineColorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_AND;
			pipelineColorBlendStateCreateInfo.attachmentCount = 1;
			pipelineColorBlendStateCreateInfo.pAttachments = &pipelineColorBlendAttachmentState;
			pipelineColorBlendStateCreateInfo.blendConstants[0] = 1;
			pipelineColorBlendStateCreateInfo.blendConstants[1] = 1;
			pipelineColorBlendStateCreateInfo.blendConstants[2] = 1;
			pipelineColorBlendStateCreateInfo.blendConstants[3] = 1;

			VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo;
			pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			pipelineDepthStencilStateCreateInfo.pNext = nullptr;
			pipelineDepthStencilStateCreateInfo.flags = 0;
			pipelineDepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
			pipelineDepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
			pipelineDepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
			pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
			pipelineDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
			pipelineDepthStencilStateCreateInfo.minDepthBounds = 0;
			pipelineDepthStencilStateCreateInfo.maxDepthBounds = 1.0;

			graphicsPipelineCreateInfo[i].sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			graphicsPipelineCreateInfo[i].pNext = nullptr;
			graphicsPipelineCreateInfo[i].flags = 0;
			graphicsPipelineCreateInfo[i].stageCount = in.pipe.shaderCount;
			graphicsPipelineCreateInfo[i].pStages = in.pipe.shader;
			graphicsPipelineCreateInfo[i].pVertexInputState = &pipelineVertexInputStateCreateInfo;
			graphicsPipelineCreateInfo[i].pInputAssemblyState = &pipelineInputStateCreateInfo;
			graphicsPipelineCreateInfo[i].pTessellationState = nullptr;
			graphicsPipelineCreateInfo[i].pViewportState = &pipelineViewportStateCreateInfo;
			graphicsPipelineCreateInfo[i].pRasterizationState = &pipelineRasterizationStateCreateInfo;
			graphicsPipelineCreateInfo[i].pMultisampleState = &pipelineMultisampleStateCreateInfo;
			graphicsPipelineCreateInfo[i].pDepthStencilState = &pipelineDepthStencilStateCreateInfo;
			graphicsPipelineCreateInfo[i].pColorBlendState = &pipelineColorBlendStateCreateInfo;
			graphicsPipelineCreateInfo[i].pDynamicState = nullptr;
			graphicsPipelineCreateInfo[i].layout = pipelineLayout;
			graphicsPipelineCreateInfo[i].renderPass = renderpass;
			graphicsPipelineCreateInfo[i].subpass = 0;
			graphicsPipelineCreateInfo[i].basePipelineHandle = nullptr;
			graphicsPipelineCreateInfo[i].basePipelineIndex = 0;
		}
		
		lastResult = vkCreateGraphicsPipelines(device, nullptr, size, graphicsPipelineCreateInfo, nullptr, pipelines);
		CHECKFAIL;
	}
}