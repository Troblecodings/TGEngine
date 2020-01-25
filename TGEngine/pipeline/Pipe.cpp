#include "Pipe.hpp"
#include "RenderPass.hpp"
#include "Descriptors.hpp"
#include "PrePipeline.hpp"
#include "window/Window.hpp"

namespace tge::pip {

	VkPipeline defaultPipeline;

	void createPipelines(PipelineInputInfo* input, uint32_t size, VkPipeline* pipelines) {

		VkGraphicsPipelineCreateInfo* graphicsPipelineCreateInfo = new VkGraphicsPipelineCreateInfo[size];
		VkVertexInputBindingDescription* vertexInputBindingDescriptor = new VkVertexInputBindingDescription[size];
		VkPipelineVertexInputStateCreateInfo* pipelineVertexInputStateCreateInfo = new VkPipelineVertexInputStateCreateInfo[size];
		VkViewport* viewport = new VkViewport[size];
		VkRect2D* sicossor = new VkRect2D[size];
		VkPipelineViewportStateCreateInfo* pipelineViewportStateCreateInfo = new VkPipelineViewportStateCreateInfo[size];
		VkPipelineRasterizationStateCreateInfo* pipelineRasterizationStateCreateInfo = new VkPipelineRasterizationStateCreateInfo[size];

		VkPipelineInputAssemblyStateCreateInfo pipelineInputStateCreateInfo;
		pipelineInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		pipelineInputStateCreateInfo.pNext = nullptr;
		pipelineInputStateCreateInfo.flags = 0;
		pipelineInputStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		pipelineInputStateCreateInfo.primitiveRestartEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo;
		pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		pipelineMultisampleStateCreateInfo.pNext = nullptr;
		pipelineMultisampleStateCreateInfo.flags = 0;
		pipelineMultisampleStateCreateInfo.rasterizationSamples = usedSampleFlag;
		pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
		pipelineMultisampleStateCreateInfo.minSampleShading = 1.0f;
		pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
		pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
		pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState;
		pipelineColorBlendAttachmentState.blendEnable = VK_TRUE;
		pipelineColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		pipelineColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		pipelineColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
		pipelineColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		pipelineColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		pipelineColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
		pipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

		VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo;
		pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		pipelineColorBlendStateCreateInfo.pNext = nullptr;
		pipelineColorBlendStateCreateInfo.flags = 0;
		pipelineColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
		pipelineColorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_CLEAR;
		pipelineColorBlendStateCreateInfo.attachmentCount = 1;
		pipelineColorBlendStateCreateInfo.pAttachments = &pipelineColorBlendAttachmentState;
		pipelineColorBlendStateCreateInfo.blendConstants[0] = VK_COMPONENT_SWIZZLE_IDENTITY;
		pipelineColorBlendStateCreateInfo.blendConstants[1] = VK_COMPONENT_SWIZZLE_IDENTITY;
		pipelineColorBlendStateCreateInfo.blendConstants[2] = VK_COMPONENT_SWIZZLE_IDENTITY;
		pipelineColorBlendStateCreateInfo.blendConstants[3] = VK_COMPONENT_SWIZZLE_IDENTITY;

		VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo;
		pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		pipelineDepthStencilStateCreateInfo.pNext = nullptr;
		pipelineDepthStencilStateCreateInfo.flags = 0;
		pipelineDepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
		pipelineDepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
		pipelineDepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
		pipelineDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
		pipelineDepthStencilStateCreateInfo.minDepthBounds = -1.0;
		pipelineDepthStencilStateCreateInfo.maxDepthBounds = 1.0;

		pipelineDepthStencilStateCreateInfo.front.failOp = VK_STENCIL_OP_ZERO;
		pipelineDepthStencilStateCreateInfo.front.passOp = VK_STENCIL_OP_ZERO;
		pipelineDepthStencilStateCreateInfo.front.depthFailOp = VK_STENCIL_OP_ZERO;
		pipelineDepthStencilStateCreateInfo.front.compareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		pipelineDepthStencilStateCreateInfo.front.compareMask = 0;
		pipelineDepthStencilStateCreateInfo.front.writeMask = 0;
		pipelineDepthStencilStateCreateInfo.front.reference = 0;

		pipelineDepthStencilStateCreateInfo.back.failOp = VK_STENCIL_OP_ZERO;
		pipelineDepthStencilStateCreateInfo.back.passOp = VK_STENCIL_OP_ZERO;
		pipelineDepthStencilStateCreateInfo.back.depthFailOp = VK_STENCIL_OP_ZERO;
		pipelineDepthStencilStateCreateInfo.back.compareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		pipelineDepthStencilStateCreateInfo.back.compareMask = 0;
		pipelineDepthStencilStateCreateInfo.back.writeMask = 0;
		pipelineDepthStencilStateCreateInfo.back.reference = 0;

		VkPipelineTessellationStateCreateInfo pipelineTessellationStateCreateInfo;
		pipelineTessellationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		pipelineTessellationStateCreateInfo.pNext = nullptr;
		pipelineTessellationStateCreateInfo.flags = 0;
		pipelineTessellationStateCreateInfo.patchControlPoints = 1;

		for (size_t i = 0; i < size; i++) {
			PipelineInputInfo in = input[i];

			vertexInputBindingDescriptor[i].binding = 0;
			vertexInputBindingDescriptor[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			vertexInputBindingDescriptor[i].stride = in.stride;

			pipelineVertexInputStateCreateInfo[i].sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			pipelineVertexInputStateCreateInfo[i].pNext = nullptr;
			pipelineVertexInputStateCreateInfo[i].flags = 0;
			pipelineVertexInputStateCreateInfo[i].vertexBindingDescriptionCount = 1;
			pipelineVertexInputStateCreateInfo[i].pVertexBindingDescriptions = &vertexInputBindingDescriptor[i];
			pipelineVertexInputStateCreateInfo[i].vertexAttributeDescriptionCount = in.pipe.inputCount;
			pipelineVertexInputStateCreateInfo[i].pVertexAttributeDescriptions = in.pipe.inputs;

			viewport[i].x = 0;
			viewport[i].y = 0;
			viewport[i].width = in.width;
			viewport[i].height = in.height;
			viewport[i].minDepth = 0;
			viewport[i].maxDepth = 1.0;

			sicossor[i].offset.x = 0;
			sicossor[i].offset.y = 0;
			sicossor[i].extent.width = viewport->width;
			sicossor[i].extent.height = viewport->height;

			pipelineViewportStateCreateInfo[i].sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			pipelineViewportStateCreateInfo[i].pNext = nullptr;
			pipelineViewportStateCreateInfo[i].flags = 0;
			pipelineViewportStateCreateInfo[i].viewportCount = 1;
			pipelineViewportStateCreateInfo[i].pViewports = &viewport[i];
			pipelineViewportStateCreateInfo[i].scissorCount = 1;
			pipelineViewportStateCreateInfo[i].pScissors = &sicossor[i];

			pipelineRasterizationStateCreateInfo[i].sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			pipelineRasterizationStateCreateInfo[i].pNext = nullptr;
			pipelineRasterizationStateCreateInfo[i].flags = 0;
			pipelineRasterizationStateCreateInfo[i].depthClampEnable = VK_TRUE;
			pipelineRasterizationStateCreateInfo[i].rasterizerDiscardEnable = VK_FALSE;
			pipelineRasterizationStateCreateInfo[i].polygonMode = in.polygonMode;
			pipelineRasterizationStateCreateInfo[i].cullMode = in.cullMode;
			pipelineRasterizationStateCreateInfo[i].frontFace = VK_FRONT_FACE_CLOCKWISE;
			pipelineRasterizationStateCreateInfo[i].depthBiasEnable = VK_FALSE;
			pipelineRasterizationStateCreateInfo[i].depthBiasConstantFactor = 0;
			pipelineRasterizationStateCreateInfo[i].depthBiasClamp = 0;
			pipelineRasterizationStateCreateInfo[i].depthBiasSlopeFactor = 1;
			pipelineRasterizationStateCreateInfo[i].lineWidth = in.lineWidth;

			graphicsPipelineCreateInfo[i].sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			graphicsPipelineCreateInfo[i].pNext = nullptr;
			graphicsPipelineCreateInfo[i].flags = 0;
			graphicsPipelineCreateInfo[i].stageCount = in.pipe.shaderCount;
			graphicsPipelineCreateInfo[i].pStages = in.pipe.shader;
			graphicsPipelineCreateInfo[i].pVertexInputState = &pipelineVertexInputStateCreateInfo[i];
			graphicsPipelineCreateInfo[i].pInputAssemblyState = &pipelineInputStateCreateInfo;
			graphicsPipelineCreateInfo[i].pTessellationState = &pipelineTessellationStateCreateInfo;
			graphicsPipelineCreateInfo[i].pViewportState = &pipelineViewportStateCreateInfo[i];
			graphicsPipelineCreateInfo[i].pRasterizationState = &pipelineRasterizationStateCreateInfo[i];
			graphicsPipelineCreateInfo[i].pMultisampleState = &pipelineMultisampleStateCreateInfo;
			graphicsPipelineCreateInfo[i].pDepthStencilState = &pipelineDepthStencilStateCreateInfo;
			graphicsPipelineCreateInfo[i].pColorBlendState = &pipelineColorBlendStateCreateInfo;
			graphicsPipelineCreateInfo[i].pDynamicState = nullptr;
			graphicsPipelineCreateInfo[i].layout = pipelineLayout;
			graphicsPipelineCreateInfo[i].renderPass = renderPass;
			graphicsPipelineCreateInfo[i].subpass = 0;
			graphicsPipelineCreateInfo[i].basePipelineHandle = VK_NULL_HANDLE;
			graphicsPipelineCreateInfo[i].basePipelineIndex = 0;
		}

		CHECKFAIL(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, size, graphicsPipelineCreateInfo, nullptr, pipelines));
	}

	void initPipelines() {

		//TODO Autogenerate pipelines
		PipelineInputInfo pipelineInputInfo;
		pipelineInputInfo.pipe = TopDownPipe;
		pipelineInputInfo.stride = sizeof(glm::vec4);
		pipelineInputInfo.width = win::mainWindowWidth;
		pipelineInputInfo.height = win::mainWindowHeight;
		pipelineInputInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		pipelineInputInfo.polygonMode = VK_POLYGON_MODE_FILL;
		pipelineInputInfo.lineWidth = 1;

		createPipelines(&pipelineInputInfo, 1, &defaultPipeline);
	}
}