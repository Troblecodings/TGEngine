#include "RenderPass.h"

namespace Pipeline {

	void createRenderPass(RenderPass render_pass) {
		VkPipelineVertexInputStateCreateInfo vert_inp_info = {};
		vert_inp_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vert_inp_info.vertexBindingDescriptionCount = 0;
		vert_inp_info.vertexAttributeDescriptionCount = 0;
		vert_inp_info.pVertexBindingDescriptions = nullptr;
		vert_inp_info.pVertexAttributeDescriptions = nullptr;

		VkPipelineInputAssemblyStateCreateInfo in_ass_info = {};
		in_ass_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		in_ass_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		in_ass_info.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport;
		viewport.x = 0;
		viewport.y = 0;
		viewport.width = render_pass.window->size.width;
		viewport.height = render_pass.window->size.height;
		viewport.maxDepth = 1;
		viewport.minDepth = 0;

		VkRect2D rect;
		rect.offset = { 0,0 };
		rect.extent = render_pass.window->size;

		VkPipelineViewportStateCreateInfo viewport_create_info = {};
		viewport_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewport_create_info.pScissors = &rect;
		viewport_create_info.pViewports = &viewport;
		viewport_create_info.scissorCount = 1;
		viewport_create_info.viewportCount = 1;

		VkPipelineRasterizationStateCreateInfo rastera_info = {};
		rastera_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rastera_info.depthClampEnable = VK_FALSE;
		rastera_info.rasterizerDiscardEnable = VK_FALSE;
		rastera_info.polygonMode = VK_POLYGON_MODE_FILL;
		rastera_info.cullMode = VK_CULL_MODE_NONE;
		rastera_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rastera_info.depthBiasEnable = VK_FALSE;
		rastera_info.depthBiasConstantFactor = 0;
		rastera_info.depthBiasClamp = 0;
		rastera_info.depthBiasSlopeFactor = 0;
		rastera_info.lineWidth = 1;

		VkPipelineMultisampleStateCreateInfo pipl_multisample = {};
		pipl_multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		pipl_multisample.alphaToCoverageEnable = VK_FALSE;
		pipl_multisample.alphaToOneEnable = VK_FALSE;
		pipl_multisample.minSampleShading = 1;
		pipl_multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		pipl_multisample.sampleShadingEnable = VK_FALSE;
		pipl_multisample.pSampleMask = nullptr;

		VkPipelineColorBlendAttachmentState state;
		state.blendEnable = VK_TRUE;
		state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
		state.colorBlendOp = VK_BLEND_OP_ADD;
		state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		state.alphaBlendOp = VK_BLEND_OP_ADD;
		state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

		VkPipelineColorBlendStateCreateInfo color_blend_create_info = {};
		color_blend_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		color_blend_create_info.logicOpEnable = VK_FALSE;
		color_blend_create_info.logicOp = VK_LOGIC_OP_NO_OP;
		color_blend_create_info.attachmentCount = 1;
		color_blend_create_info.pAttachments = &state;
		color_blend_create_info.blendConstants[0] = 0;
		color_blend_create_info.blendConstants[1] = 0;
		color_blend_create_info.blendConstants[2] = 0;
		color_blend_create_info.blendConstants[3] = 0;

		VkPipelineLayoutCreateInfo layout_create_info = {};
		layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		handel(vkCreatePipelineLayout(*render_pass.device->device, &layout_create_info, nullptr, render_pass.layout));

		VkAttachmentDescription att_desc;
		att_desc.flags = 0;
		att_desc.format = render_pass.device->prefered_format;
		att_desc.samples = VK_SAMPLE_COUNT_1_BIT;
		att_desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		att_desc.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		att_desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		att_desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		att_desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		att_desc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference reference_att;
		reference_att.attachment = 0;
		reference_att.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass_desc = {};
		subpass_desc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass_desc.colorAttachmentCount = 1;
		subpass_desc.pColorAttachments = &reference_att;

		VkSubpassDependency subpass = {};
		subpass.srcSubpass = VK_SUBPASS_EXTERNAL;
		subpass.dstSubpass = 0;
		subpass.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpass.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpass.srcAccessMask = 0;
		subpass.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		subpass.dependencyFlags = 0;

		VkRenderPassCreateInfo render_pass_create_info = {};
		render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		render_pass_create_info.attachmentCount = 1;
		render_pass_create_info.pAttachments = &att_desc;
		render_pass_create_info.subpassCount = 1;
		render_pass_create_info.pSubpasses = &subpass_desc;
		render_pass_create_info.dependencyCount = 1;
		render_pass_create_info.pDependencies = &subpass;

		handel(vkCreateRenderPass(*render_pass.device->device, &render_pass_create_info, nullptr, render_pass.render_pass));
	}

	void destroyRenderPass(RenderPass render_pass) {
		vkDestroyPipelineLayout(*render_pass.device->device, *render_pass.layout, nullptr);
		vkDestroyRenderPass(*render_pass.device->device, *render_pass.render_pass, nullptr);
	}

}