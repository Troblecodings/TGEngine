#include "Pipeline.h"

namespace Pipeline {

	void createPipeline(Pipe* pipeline) {

		pipeline->command_buffer = new vector<VkCommandBuffer>;
		pipeline->frame_buffer = new vector<VkFramebuffer>;

		VkPipelineMultisampleStateCreateInfo pipl_multisample = {};
		pipl_multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		pipl_multisample.alphaToCoverageEnable = VK_FALSE;
		pipl_multisample.alphaToOneEnable = VK_FALSE;
		pipl_multisample.minSampleShading = 1;
		pipl_multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		pipl_multisample.sampleShadingEnable = VK_FALSE;
		pipl_multisample.pSampleMask = nullptr;

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

		VkPipelineVertexInputStateCreateInfo vert_inp_info = {};
		vert_inp_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		
		vector<VkVertexInputBindingDescription> bindings = {};
		vert_inp_info.vertexBindingDescriptionCount = bindings.size();
		vert_inp_info.pVertexBindingDescriptions = bindings.data();

		vector<VkVertexInputAttributeDescription> atributs = {};
		vert_inp_info.vertexAttributeDescriptionCount = atributs.size();
		vert_inp_info.pVertexAttributeDescriptions = atributs.data();

		VkPipelineInputAssemblyStateCreateInfo in_asm_info = {};
		in_asm_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		in_asm_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		in_asm_info.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport;
		viewport.x = 0;
		viewport.y = 0;
		viewport.width = pipeline->window->size.width;
		viewport.height = pipeline->window->size.height;
		viewport.maxDepth = 1;
		viewport.minDepth = 0;

		VkRect2D rect;
		rect.offset = { 0,0 };
		rect.extent = pipeline->window->size;

		VkPipelineViewportStateCreateInfo viewport_create_info = {};
		viewport_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewport_create_info.pScissors = &rect;
		viewport_create_info.pViewports = &viewport;
		viewport_create_info.scissorCount = 1;
		viewport_create_info.viewportCount = 1;

		VkGraphicsPipelineCreateInfo pipline_create_info = {};
		pipline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipline_create_info.stageCount = pipeline->shader->size();
		pipline_create_info.pStages = pipeline->shader->data();
		pipline_create_info.pVertexInputState = &vert_inp_info;
		pipline_create_info.pInputAssemblyState = &in_asm_info;
		pipline_create_info.pTessellationState = nullptr;
	    pipline_create_info.pViewportState = &viewport_create_info;
		pipline_create_info.pRasterizationState = &rastera_info;
		pipline_create_info.pMultisampleState = &pipl_multisample;
		pipline_create_info.pDepthStencilState = nullptr;
		pipline_create_info.pColorBlendState = &color_blend_create_info;
		pipline_create_info.pDynamicState = nullptr;
		pipline_create_info.layout = *pipeline->render_pass->layout;
		pipline_create_info.renderPass = *pipeline->render_pass->render_pass;
		pipline_create_info.subpass = 0;
		pipline_create_info.basePipelineHandle = VK_NULL_HANDLE;
		pipline_create_info.basePipelineIndex = -1;



		pipeline->pipeline = new VkPipeline;
    	handel(vkCreateGraphicsPipelines(*pipeline->device, VK_NULL_HANDLE, 1, &pipline_create_info, nullptr, pipeline->pipeline));

		pipeline->frame_buffer->resize(pipeline->swapchain->image_count);
		for (size_t i = 0; i < pipeline->swapchain->image_count; i++)
		{
			VkFramebufferCreateInfo framebuffer_create_info = {};
			framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebuffer_create_info.renderPass = *pipeline->render_pass->render_pass;
			framebuffer_create_info.attachmentCount = 1;
			framebuffer_create_info.pAttachments = &pipeline->swapchain->image_view_swapchain[i];
			framebuffer_create_info.width = pipeline->window->size.width;
			framebuffer_create_info.height = pipeline->window->size.height;
			framebuffer_create_info.layers = 1;

			handel(vkCreateFramebuffer(*pipeline->device, &framebuffer_create_info, nullptr, &pipeline->frame_buffer->data()[i]));
		}

		VkCommandPoolCreateInfo command_pool_create = {};
		command_pool_create.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		command_pool_create.queueFamilyIndex = 0;

		pipeline->command_pool = new VkCommandPool;
		handel(vkCreateCommandPool(*pipeline->device, &command_pool_create, nullptr, pipeline->command_pool));

		VkCommandBufferAllocateInfo commandbuffer_allocate_info = {};
		commandbuffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandbuffer_allocate_info.commandPool = *pipeline->command_pool;
		commandbuffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandbuffer_allocate_info.commandBufferCount = pipeline->swapchain->image_view_swapchain.size();

		pipeline->command_buffer->resize(pipeline->swapchain->image_view_swapchain.size());
		handel(vkAllocateCommandBuffers(*pipeline->device, &commandbuffer_allocate_info, pipeline->command_buffer->data()));

		VkCommandBufferBeginInfo command_begin_info = {};
		command_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		command_begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		for (size_t i = 0; i < pipeline->command_buffer->size(); i++)
		{
			handel(vkBeginCommandBuffer(pipeline->command_buffer->data()[i], &command_begin_info));

			VkRenderPassBeginInfo begin_render_pass = {};
			begin_render_pass.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			begin_render_pass.renderPass = *pipeline->render_pass->render_pass;
			begin_render_pass.framebuffer = pipeline->frame_buffer->data()[i];
			begin_render_pass.renderArea = rect;

			VkClearValue clear_color = { };

			begin_render_pass.clearValueCount = 1;
			begin_render_pass.pClearValues = &clear_color;

			vkCmdBeginRenderPass(pipeline->command_buffer->data()[i], &begin_render_pass, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(pipeline->command_buffer->data()[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline->pipeline);

			vkCmdDraw(pipeline->command_buffer->data()[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(pipeline->command_buffer->data()[i]);

			handel(vkEndCommandBuffer(pipeline->command_buffer->data()[i]));
		}

		VkSemaphoreCreateInfo semaphore_create_info = {};
		semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphore_create_info.flags = 0;
		semaphore_create_info.pNext = nullptr;

		pipeline->available = new VkSemaphore;
		pipeline->end = new VkSemaphore;
		handel(vkCreateSemaphore(*pipeline->device, &semaphore_create_info, nullptr, pipeline->available));
		handel(vkCreateSemaphore(*pipeline->device, &semaphore_create_info, nullptr, pipeline->end));
	}

	void destroyPipeline(Pipe* pipeline) {
		vkDestroySemaphore(*pipeline->device, *pipeline->end, nullptr);
		vkDestroySemaphore(*pipeline->device, *pipeline->available, nullptr);

		vkDestroyCommandPool(*pipeline->device, *pipeline->command_pool, nullptr);

		vkDestroyPipeline(*pipeline->device, *pipeline->pipeline, nullptr);
	}

}