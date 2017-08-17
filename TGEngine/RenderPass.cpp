#include "RenderPass.h"

namespace Pipeline {

	void createRenderPass(RenderPass* render_pass) {

		VkPipelineLayoutCreateInfo layout_create_info = {};
		layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layout_create_info.pNext = nullptr;
		layout_create_info.flags = 0;
		layout_create_info.setLayoutCount = 0;
		layout_create_info.pSetLayouts = nullptr;

		vector<VkPushConstantRange> push(0);
		layout_create_info.pushConstantRangeCount = push.size();
		layout_create_info.pPushConstantRanges = push.data();

		render_pass->layout = new VkPipelineLayout;
		handel(vkCreatePipelineLayout(*render_pass->device->device, &layout_create_info, nullptr, render_pass->layout));

		VkAttachmentDescription att_desc;
		att_desc.flags = 0;
		att_desc.format = render_pass->device->prefered_format;
		att_desc.samples = VK_SAMPLE_COUNT_1_BIT;
		att_desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		att_desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
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

		render_pass->render_pass = new VkRenderPass;
		handel(vkCreateRenderPass(*render_pass->device->device, &render_pass_create_info, nullptr, render_pass->render_pass));
	}

	void destroyRenderPass(RenderPass* render_pass) {
		vkDestroyPipelineLayout(*render_pass->device->device, *render_pass->layout, nullptr);
		vkDestroyRenderPass(*render_pass->device->device, *render_pass->render_pass, nullptr);
	}

}