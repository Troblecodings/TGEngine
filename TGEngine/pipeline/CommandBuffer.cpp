#include "CommandBuffer.hpp"


VkCommandPool command_pool;
std::vector<VkCommandBuffer> command_buffers;
VkDeviceSize offsets = 0;
bool started = true;

void createCommandBuffer() {
	command_buffers.resize(image_count);

	if (!command_pool) {
		VkCommandPoolCreateInfo commmand_pool_create_info = {
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			nullptr,
			VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			queue_index
		};

		last_result = vkCreateCommandPool(device, &commmand_pool_create_info, nullptr, &command_pool);
		HANDEL(last_result)
	}

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

void singleTimeCommand() {
	VkCommandBufferAllocateInfo command_buffer_allocate_info = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		nullptr,
		command_pool,
		VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		1
	};
	VkCommandBuffer buffer;
	last_result = vkAllocateCommandBuffers(device, &command_buffer_allocate_info, &buffer);
	HANDEL(last_result)

		VkCommandBufferBeginInfo beginInfo = {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			nullptr,
			VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
			nullptr
	};
	last_result = vkBeginCommandBuffer(buffer, &beginInfo);
	HANDEL(last_result)

		for each(Texture* tex in texture_buffers) {
			tex->addBarrier(buffer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

			VkBufferImageCopy image_copy = {
				0,
				0,
				0,
			{
				VK_IMAGE_ASPECT_COLOR_BIT,
				0,
				0,
				1
			},
					{
						0,
						0,
						0
					},
					{
						tex->width,
						tex->height,
						1
					}
			};

			vkCmdCopyBufferToImage(
				buffer,
				tex->buffer,
				tex->image,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&image_copy
			);

			tex->addBarrier(buffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		}

	last_result = vkEndCommandBuffer(buffer);
	HANDEL(last_result)

		VkSubmitInfo submitInfo = {
			VK_STRUCTURE_TYPE_SUBMIT_INFO,
			nullptr,
			0,
			nullptr,
			nullptr,
			1,
			&buffer,
			0,
			nullptr,
	};

	vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(queue);

	vkFreeCommandBuffers(device, command_pool, 1, &buffer);

	for each(Texture* tex in texture_buffers) {
		destroyBufferofTexture(tex);
	}
}

void fillCommandBuffer(VertexBuffer* vbuffer, uint32_t index) {
	VkClearValue clear_color = {
		{
			1,
			1,
			1,
			1
		}
	};

	VkCommandBuffer buffer = command_buffers[index];

	VkCommandBufferInheritanceInfo command_buffer_inheritance_info = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
			nullptr,
			render_pass,
			0,
			frame_buffer[index],
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
		frame_buffer[index],
		{
			0,
			0,
			window_list[0]->width,
			window_list[0]->height
		},
		1,
		&clear_color
	};
	vkCmdBeginRenderPass(buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

	vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptor_set, 0, nullptr);

	vkCmdBindVertexBuffers(buffer, 0, 1, &buffers[vbuffer->vertex_buffer_index], &offsets);

	vkCmdDraw(buffer, vbuffer->count_of_points, 1, 0, 0);

	vkCmdEndRenderPass(buffer);

	last_result = vkEndCommandBuffer(buffer);
	HANDEL(last_result)
}

void submitWithoutTexture(VertexBuffer* buffer, uint32_t index) {
/*	uint32_t max = 101;
	vkCmdPushConstants(command_buffers[index], layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(uint32_t), &max);

	vkCmdBindVertexBuffers(command_buffers[index], 0, 1, &buffers[buffer->vertex_buffer_index], &offsets);

	vkCmdDraw(command_buffers[index], offsets - buffer->count_of_points, 1, 0, 0);
	offsets = VERTEX_SIZE * buffer->count_of_points;*/
}

void endCommandBuffer(VertexBuffer* vbuffer, uint32_t index) {
	/*VkCommandBuffer buffer = command_buffers[index];
	vkCmdBindVertexBuffers(buffer, 0, 1, &buffers[vbuffer->vertex_buffer_index], &offsets);

	vkCmdDraw(buffer, offsets - vbuffer->count_of_points, 1, 0, 0);

	vkCmdEndRenderPass(buffer);

	last_result = vkEndCommandBuffer(buffer);
	HANDEL(last_result)*/
}

void destroyCommandBuffer() {
	vkFreeCommandBuffers(device, command_pool, command_buffers.size(), command_buffers.data());
	vkDestroyCommandPool(device, command_pool, nullptr);
}