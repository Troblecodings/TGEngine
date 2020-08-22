#include "../../public/pipeline/Pipe.hpp"
#include "../../public/pipeline/RenderPass.hpp"
#include "../../public/pipeline/Descriptors.hpp"
#include "../../public/pipeline/PrePipeline.hpp"
#include "../../public/pipeline/window/Window.hpp"

namespace tge::pip {

	VkPipeline defaultPipeline[PIPELINE_COUNT];

	void createPipelines(PipelineInputInfo* input, uint32_t size, VkPipeline* pipelines) {

		VkGraphicsPipelineCreateInfo* graphicsPipelineCreateInfo = new VkGraphicsPipelineCreateInfo[size];
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
		pipelineColorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_OR;
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
		pipelineDepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
		pipelineDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
		pipelineDepthStencilStateCreateInfo.minDepthBounds = 0.0;
		pipelineDepthStencilStateCreateInfo.maxDepthBounds = 1.0;

		pipelineDepthStencilStateCreateInfo.front.failOp = VK_STENCIL_OP_ZERO;
		pipelineDepthStencilStateCreateInfo.front.passOp = VK_STENCIL_OP_REPLACE;
		pipelineDepthStencilStateCreateInfo.front.depthFailOp = VK_STENCIL_OP_ZERO;
		pipelineDepthStencilStateCreateInfo.front.compareOp = VK_COMPARE_OP_LESS;
		pipelineDepthStencilStateCreateInfo.front.compareMask = 0;
		pipelineDepthStencilStateCreateInfo.front.writeMask = 0;
		pipelineDepthStencilStateCreateInfo.front.reference = 0;

		pipelineDepthStencilStateCreateInfo.back.failOp = VK_STENCIL_OP_ZERO;
		pipelineDepthStencilStateCreateInfo.back.passOp = VK_STENCIL_OP_REPLACE;
		pipelineDepthStencilStateCreateInfo.back.depthFailOp = VK_STENCIL_OP_ZERO;
		pipelineDepthStencilStateCreateInfo.back.compareOp = VK_COMPARE_OP_LESS;
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

			pipelineVertexInputStateCreateInfo[i].sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			pipelineVertexInputStateCreateInfo[i].pNext = nullptr;
			pipelineVertexInputStateCreateInfo[i].flags = 0;
			pipelineVertexInputStateCreateInfo[i].vertexBindingDescriptionCount = in.inputBindingCount;
			pipelineVertexInputStateCreateInfo[i].pVertexBindingDescriptions = in.inputBindings;
			pipelineVertexInputStateCreateInfo[i].vertexAttributeDescriptionCount = in.pipe.inputCount;
			pipelineVertexInputStateCreateInfo[i].pVertexAttributeDescriptions = in.pipe.inputs;

			viewport[i].x = 0;
			viewport[i].y = 0;
			viewport[i].width = (float)tge::win::mainWindowWidth;
			viewport[i].height = (float)tge::win::mainWindowHeight;
			viewport[i].minDepth = 0;
			viewport[i].maxDepth = 1.0;

			sicossor[i].offset.x = 0;
			sicossor[i].offset.y = 0;
			sicossor[i].extent.width = tge::win::mainWindowWidth;
			sicossor[i].extent.height = tge::win::mainWindowHeight;

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
			pipelineRasterizationStateCreateInfo[i].depthBiasSlopeFactor = 0;
			pipelineRasterizationStateCreateInfo[i].lineWidth = 1;

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
		PipelineInputInfo pipelineInputInfo[PIPELINE_COUNT];
		pipelineInputInfo[0].pipe = TopDownPipe;
		pipelineInputInfo[0].cullMode = VK_CULL_MODE_BACK_BIT;
		pipelineInputInfo[0].polygonMode = VK_POLYGON_MODE_FILL;
		pipelineInputInfo[0].inputBindingCount = 1;
		pipelineInputInfo[0].inputBindings = new VertexInputBinding[1];
		pipelineInputInfo[0].inputBindings[0].binding = 0;
		pipelineInputInfo[0].inputBindings[0].stride = sizeof(glm::vec4);
		pipelineInputInfo[0].inputBindings[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		TopDownInstancedPipe.inputs[2].binding = 1;
		TopDownInstancedPipe.inputs[3].binding = 1;
		TopDownInstancedPipe.inputs[4].binding = 1;
		TopDownInstancedPipe.inputs[5].binding = 1;
		pipelineInputInfo[1].pipe = TopDownInstancedPipe;
		pipelineInputInfo[1].cullMode = VK_CULL_MODE_BACK_BIT;
		pipelineInputInfo[1].polygonMode = VK_POLYGON_MODE_FILL;
		pipelineInputInfo[1].inputBindingCount = 2;
		pipelineInputInfo[1].inputBindings = new VertexInputBinding[2];
		pipelineInputInfo[1].inputBindings[0].binding = 0;
		pipelineInputInfo[1].inputBindings[0].stride = sizeof(glm::vec4);
		pipelineInputInfo[1].inputBindings[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		pipelineInputInfo[1].inputBindings[1].binding = 1;
		pipelineInputInfo[1].inputBindings[1].stride = 6*4;
		pipelineInputInfo[1].inputBindings[1].inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

		createPipelines(pipelineInputInfo, PIPELINE_COUNT, defaultPipeline);
	}

	void destroyPipelines() {
		for (size_t i = 0; i < PIPELINE_COUNT; i++) {
			vkDestroyPipeline(device, defaultPipeline[i], nullptr);
		}
	}
}