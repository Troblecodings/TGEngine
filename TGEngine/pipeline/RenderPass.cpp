#include "RenderPass.hpp"

VkRenderPass render_pass;

void createRenderpass() {
	//Attachment for color images
	VkAttachmentDescription attachment_description = {
		0,
		used_format.format,
		VK_SAMPLE_COUNT_1_BIT,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};

	//Attachment for depth images
	VkAttachmentDescription depth_attachment_description = {
		0,
		used_depth_format,
		VK_SAMPLE_COUNT_1_BIT,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
	};

	//Input attachment
	VkAttachmentReference attachment_reference = {
		0,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	};

	//Depth attachment
	VkAttachmentReference depth_attachment_reference = {
		1,
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
	};

	//Subpass
	VkSubpassDescription subpass_description = {
		0,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		0,
		nullptr,
		1,
		&attachment_reference,
		nullptr,
		&depth_attachment_reference,
		0,
		nullptr
	};

	VkSubpassDependency subpass_dependency = {
		0,
		VK_SUBPASS_EXTERNAL,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
		VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
		0,
		0,
		0
	};

	VkAttachmentDescription attachments[2] = { attachment_description, depth_attachment_description };

	//Renderpass
	VkRenderPassCreateInfo render_pass_create_info = {
		VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
	    nullptr,
	    0,
	    2,
		attachments,
	    1,
	    &subpass_description,
	    1,
	    &subpass_dependency
	};

	last_result = vkCreateRenderPass(device, &render_pass_create_info, nullptr, &render_pass);
	HANDEL(last_result);
}

void destroyRenderPass() {
	vkDestroyRenderPass(device, render_pass, nullptr);
}