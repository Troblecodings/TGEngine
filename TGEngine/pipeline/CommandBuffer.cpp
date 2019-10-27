#include "CommandBuffer.hpp"
#include "../gamecontent/Actor.hpp"

VkCommandPool command_pool;
VkFence single_time_command_ready;
std::vector<VkCommandBuffer> command_buffers;
VkDeviceSize offsets = 0;
bool started = true;

void createCommandBuffer() {
	command_buffers.resize(imagecount + 1);

	if(!command_pool) {
		VkCommandPoolCreateInfo commmand_pool_create_info = {
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			nullptr,
			VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			queueIndex
		};

		lastResult = vkCreateCommandPool(device, &commmand_pool_create_info, nullptr, &command_pool);
		HANDEL(lastResult)

			vlibCommandBufferAllocateInfo.commandPool = command_pool;
	}

	vlibCommandBufferAllocateInfo.commandBufferCount = imagecount;
	lastResult = vkAllocateCommandBuffers(device, &vlibCommandBufferAllocateInfo, command_buffers.data());
	HANDEL(lastResult)

		vlibCommandBufferAllocateInfo.commandBufferCount = 1;
	lastResult = vkAllocateCommandBuffers(device, &vlibCommandBufferAllocateInfo, &SINGELTIME_COMMAND_BUFFER);
	HANDEL(lastResult)

		VkFenceCreateInfo fence_create_info = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
	lastResult = vkCreateFence(device, &fence_create_info, nullptr, &single_time_command_ready);
	HANDEL(lastResult)
}

void startSingleTimeCommand() {
	lastResult = vkResetFences(device, 1, &single_time_command_ready);
	HANDEL(lastResult)

		vlibCommandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vlibCommandBufferBeginInfo.pInheritanceInfo = nullptr;
	lastResult = vkBeginCommandBuffer(SINGELTIME_COMMAND_BUFFER, &vlibCommandBufferBeginInfo);
	HANDEL(lastResult);
}

void endSingleTimeCommand() {
	lastResult = vkEndCommandBuffer(SINGELTIME_COMMAND_BUFFER);
	HANDEL(lastResult);

	VkSubmitInfo submitInfo = {
		VK_STRUCTURE_TYPE_SUBMIT_INFO,
		nullptr,
		0,
		nullptr,
		nullptr,
		1,
		&SINGELTIME_COMMAND_BUFFER,
		0,
		nullptr,
	};

	lastResult = vkQueueSubmit(queue, 1, &submitInfo, single_time_command_ready);
	HANDEL(lastResult)

		lastResult = vkWaitForFences(device, 1, &single_time_command_ready, VK_TRUE, UINT64_MAX);
	HANDEL(lastResult)
}

void startupCommands() {
	startSingleTimeCommand();

	vlibImageMemoryBarrier.subresourceRange.levelCount = 1;
	vlibImageMemoryBarrier.subresourceRange.baseMipLevel = 0;
	ADD_IMAGE_MEMORY_BARRIER(SINGELTIME_COMMAND_BUFFER, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, color_image, 0, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT)

		vlibImageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	ADD_IMAGE_MEMORY_BARRIER(SINGELTIME_COMMAND_BUFFER, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, depth_image, 0, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT)

		for each(StagingBuffer * buf in staging_buffer) {
			vlibBufferCopy.dstOffset = vlibBufferCopy.srcOffset = 0;
			vlibBufferCopy.size = buf->size;
			vkCmdCopyBuffer(
				SINGELTIME_COMMAND_BUFFER,
				buf->staging_buffer,
				*buf->destination,
				1,
				&vlibBufferCopy
			);
		}

	endSingleTimeCommand();
}

void fillCommandBuffer(IndexBuffer* ibuffer, VertexBuffer* vbuffer) {
	for(size_t i = 0; i < imagecount; i++) {
		VkCommandBuffer buffer = command_buffers[i];

		VkCommandBufferInheritanceInfo command_buffer_inheritance_info = {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
				nullptr,
				renderpass,
				0,
				frame_buffer[i],
				VK_FALSE,
				0,
				0
		};

		vlibCommandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		vlibCommandBufferBeginInfo.pInheritanceInfo = &command_buffer_inheritance_info;
		lastResult = vkBeginCommandBuffer(buffer, &vlibCommandBufferBeginInfo);
		HANDEL(lastResult);

		VkRenderPassBeginInfo render_pass_begin_info = {
			VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			nullptr,
			renderpass,
			frame_buffer[i],
			{
				0,
				0,
				(uint32_t)windowList[0]->width,
				(uint32_t)windowList[0]->height
			},
			2,
			vlibClearValues
		};
		vkCmdBeginRenderPass(buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindVertexBuffers(buffer, 0, 1, &vbuffer->vertex_buffer, &offsets);

		vkCmdBindIndexBuffer(buffer, ibuffer->index_buffer, 0, VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &mainDescriptorSet, 0, nullptr);

		vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, tge::pip::defaultPipeline);

		vkCmdDrawIndexed(buffer, ibuffer->indexCount, 1, 0, 0, 0);

		vkCmdEndRenderPass(buffer);

		lastResult = vkEndCommandBuffer(buffer);
		HANDEL(lastResult)
	}
}

void destroyCommandBuffer() {
	lastResult = vkDeviceWaitIdle(device);
	HANDEL(lastResult)
		vkFreeCommandBuffers(device, command_pool, (uint32_t)command_buffers.size(), command_buffers.data());
	vkDestroyCommandPool(device, command_pool, nullptr);
	vkDestroyFence(device, single_time_command_ready, nullptr);
}