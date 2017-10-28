#include "CommandBuffer.hpp"


VkCommandPool command_pool;
std::vector<VkCommandBuffer> command_buffers;

void createCommandBuffer() {
	command_buffers.resize(image_count);

	VkCommandPoolCreateInfo commmand_pool_create_info = {
		VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
	    nullptr,
	    0,
	    queue_index
	};

	last_result = vkCreateCommandPool(device, &commmand_pool_create_info, nullptr, &command_pool);
	HANDEL(last_result)

	VkCommandBufferAllocateInfo command_buffer_allocate_info = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
	    nullptr,
	    command_pool,
		VK_COMMAND_BUFFER_LEVEL_PRIMARY,
	    image_count
	};
	last_result = vkAllocateCommandBuffers(device, &command_buffer_allocate_info, command_buffers.data());
	HANDEL(last_result)
}

void fillCommandBuffer() {
	VkClearValue clear_color = {
		{
			1.0F, 
		    1.0F, 
		    1.0F, 
			1.0F 
		}
	};

	size_t count = 0;
	for (VkCommandBuffer buffer : command_buffers) {
		VkCommandBufferInheritanceInfo command_buffer_inheritance_info = {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
			nullptr,
			render_pass,
			0,
			frame_buffer[count],
			VK_FALSE,
			0,
			0
		};

		VkCommandBufferBeginInfo command_buffer_begin_info = {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			nullptr,
			VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
			&command_buffer_inheritance_info
		};
		last_result = vkBeginCommandBuffer(buffer, &command_buffer_begin_info);
		HANDEL(last_result)

		VkRenderPassBeginInfo render_pass_begin_info = {
			VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		    nullptr,
		    render_pass,
		    frame_buffer[count],
			{
				0,
				0,
				width,
				height
			},
		    1,
			&clear_color
		};

		vkCmdBeginRenderPass(buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
		
		vkCmdSetViewport(buffer, 0, 1, &viewport);

		vkCmdSetScissor(buffer, 0, 1, &scissor);

		vkCmdDraw(buffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(buffer);

		vkEndCommandBuffer(buffer);

		count++;
	}
}

void destroyCommandBuffer() {
	vkFreeCommandBuffers(device, command_pool, command_buffers.size(), command_buffers.data());
	vkDestroyCommandPool(device, command_pool, nullptr);
}