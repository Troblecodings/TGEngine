#include "RenderPass.hpp"

VkRenderPass render_pass;

void createRenderpass() {
	//Attachment
	VkAttachmentDescription attachment_description = {
		0,
	    VK_FORMAT_B8G8R8A8_UNORM,
		VK_SAMPLE_COUNT_1_BIT,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
	    VK_ATTACHMENT_STORE_OP_DONT_CARE,
	    VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
	    VK_IMAGE_LAYOUT_UNDEFINED,
	    VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};

	//Subpass
	VkSubpassDescription subpass_description = {
		0,
	    VK_PIPELINE_BIND_POINT_GRAPHICS,
	    0,
	    nullptr,
	    0,
	    nullptr,
	    nullptr,
	    nullptr,
	    0,
	    nullptr
	};

	VkSubpassDependency subpass_dependency = {
		0,
		VK_SUBPASS_EXTERNAL,
		VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
		VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
		VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
		VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
	    0,
	};

	//Renderpass
	VkRenderPassCreateInfo render_pass_create_info = {
		VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
	    nullptr,
	    0,
	    1,
	    &attachment_description,
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