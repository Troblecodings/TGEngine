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
		pipl_multisample.sampleShadingEnable = VK_TRUE;
		pipl_multisample.pSampleMask = nullptr;

		VkPipelineRasterizationStateCreateInfo rastera_info = {};
		rastera_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rastera_info.depthClampEnable = VK_FALSE;
		rastera_info.rasterizerDiscardEnable = VK_FALSE;
		rastera_info.polygonMode = VK_POLYGON_MODE_FILL;
		rastera_info.cullMode = VK_CULL_MODE_FRONT_AND_BACK;
		rastera_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rastera_info.depthBiasEnable = VK_FALSE;
		rastera_info.depthBiasConstantFactor = 0;
		rastera_info.depthBiasClamp = 0;
		rastera_info.depthBiasSlopeFactor = 0;
		rastera_info.lineWidth = 1;

		VkPipelineColorBlendAttachmentState state;
		state.blendEnable = VK_TRUE;
		state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
		state.colorBlendOp = VK_BLEND_OP_ADD;
		state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
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
		
		VkVertexInputBindingDescription binding = {};
		binding.binding = 0;
		binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		binding.stride = sizeof(Vertex);

		vector<VkVertexInputBindingDescription> bindings = { binding };
		vert_inp_info.vertexBindingDescriptionCount = bindings.size();
		vert_inp_info.pVertexBindingDescriptions = bindings.data();

		VkVertexInputAttributeDescription colorin = {};
		colorin.binding = binding.binding;
		colorin.location = 1;
		colorin.offset = offsetof(Vertex, color);
		colorin.format = VK_FORMAT_R32G32B32A32_SFLOAT;

		VkVertexInputAttributeDescription posd = {};
		posd.binding = binding.binding;
		posd.location = 0;
		posd.offset = offsetof(Vertex, pos);
		posd.format = VK_FORMAT_R32G32_SFLOAT;

		vector<VkVertexInputAttributeDescription> attributs = { posd, colorin };
		vert_inp_info.vertexAttributeDescriptionCount = attributs.size();
		vert_inp_info.pVertexAttributeDescriptions = attributs.data();

		VkPipelineInputAssemblyStateCreateInfo in_asm_info = {};
		in_asm_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		in_asm_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		in_asm_info.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport;
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float) pipeline->window->size.width;
		viewport.height = (float) pipeline->window->size.height;
		viewport.maxDepth = 1.0f;
		viewport.minDepth = 0.0f;

		VkRect2D rect;
		rect.offset.x = 0;
		rect.offset.y = 0;
		rect.extent = pipeline->window->size;

		VkPipelineViewportStateCreateInfo viewport_create_info = {};
		viewport_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewport_create_info.pScissors = &rect;
		viewport_create_info.pViewports = &viewport;
		viewport_create_info.scissorCount = 1;
		viewport_create_info.viewportCount = 1;

		vector<VkPipelineShaderStageCreateInfo> data(pipeline->shader.size());
		for (size_t i = 0; i < data.size(); i++)
		{
			data[i] = {};
			data[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			data[i].pName = "main";
			data[i].stage = pipeline->shader.data()[i]->bits;
			data[i].module = *pipeline->shader.data()[i]->module;
		}

		VkGraphicsPipelineCreateInfo pipline_create_info = {};
		pipline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipline_create_info.stageCount = data.size();
		pipline_create_info.pStages = data.data();
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
    	handel(vkCreateGraphicsPipelines(*pipeline->device->device, VK_NULL_HANDLE, 1, &pipline_create_info, nullptr, pipeline->pipeline));

		pipeline->frame_buffer->resize(pipeline->swapchain->image_count);
		for (size_t i = 0; i < pipeline->swapchain->image_count; i++)
		{
			VkImageView attachments[] = {
				pipeline->swapchain->image_view_swapchain[i] 
			};


			VkFramebufferCreateInfo framebuffer_create_info = {};
			framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebuffer_create_info.renderPass = *pipeline->render_pass->render_pass;
			framebuffer_create_info.attachmentCount = 1;
			framebuffer_create_info.pAttachments = attachments;
			framebuffer_create_info.width = pipeline->window->size.width;
			framebuffer_create_info.height = pipeline->window->size.height;
			framebuffer_create_info.layers = 1;

			handel(vkCreateFramebuffer(*pipeline->device->device, &framebuffer_create_info, nullptr, &pipeline->frame_buffer->data()[i]));
		}

		VkCommandPoolCreateInfo command_pool_create = {};
		command_pool_create.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		command_pool_create.queueFamilyIndex = pipeline->device->queuFamalieindex;

		pipeline->command_pool = new VkCommandPool;
		handel(vkCreateCommandPool(*pipeline->device->device, &command_pool_create, nullptr, pipeline->command_pool));

		VkCommandBufferAllocateInfo commandbuffer_allocate_info = {};
		commandbuffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandbuffer_allocate_info.commandPool = *pipeline->command_pool;
		commandbuffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandbuffer_allocate_info.commandBufferCount = pipeline->swapchain->image_view_swapchain.size();

		pipeline->command_buffer->resize(pipeline->swapchain->image_view_swapchain.size());
		handel(vkAllocateCommandBuffers(*pipeline->device->device, &commandbuffer_allocate_info, pipeline->command_buffer->data()));

		VkCommandBufferBeginInfo command_begin_info = {};
		command_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		command_begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		command_begin_info.pInheritanceInfo = nullptr;

		for (size_t i = 0; i < pipeline->command_buffer->size(); i++)
		{
			handel(vkBeginCommandBuffer((*pipeline->command_buffer)[i], &command_begin_info));

			VkRenderPassBeginInfo begin_render_pass = {};
			begin_render_pass.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			begin_render_pass.renderPass = *pipeline->render_pass->render_pass;
			begin_render_pass.framebuffer = pipeline->frame_buffer->data()[i];
			begin_render_pass.renderArea = rect;

			VkClearValue clear_color = { };
			clear_color.color = { 0, 1, 1, 1 };

			begin_render_pass.clearValueCount = 1;
			begin_render_pass.pClearValues = &clear_color;

			vkCmdBeginRenderPass((*pipeline->command_buffer)[i], &begin_render_pass, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline((*pipeline->command_buffer)[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline->pipeline);

			vkCmdSetViewport((*pipeline->command_buffer)[i], 0, 1, &viewport);

			vkCmdSetScissor((*pipeline->command_buffer)[i], 0, 1, &rect);

			VkDeviceSize size[] = { 0 };
			vkCmdBindVertexBuffers((*pipeline->command_buffer)[i], 0, 1, pipeline->buffer->buffer, size);

			vkCmdDraw((*pipeline->command_buffer)[i], pipeline->buffer->vertecies->size(), 1, 0, 0);

			vkCmdEndRenderPass((*pipeline->command_buffer)[i]);

			handel(vkEndCommandBuffer((*pipeline->command_buffer)[i]));
		}
	}

	void destroyPipeline(Pipe* pipeline) {
		vkFreeCommandBuffers(*pipeline->device->device, *pipeline->command_pool, pipeline->command_buffer->size(), pipeline->command_buffer->data());

		vkDestroyCommandPool(*pipeline->device->device, *pipeline->command_pool, nullptr);

		for (VkFramebuffer buffer : *pipeline->frame_buffer) {
			vkDestroyFramebuffer(*pipeline->device->device, buffer, nullptr);
		}

		vkDestroyPipeline(*pipeline->device->device, *pipeline->pipeline, nullptr);
	}

}