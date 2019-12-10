#include "CommandBuffer.hpp"
#include "../gamecontent/Actor.hpp"
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
		CHECKFAIL;

		vlibCommandBufferAllocateInfo.commandPool = command_pool;
	}

	vlibCommandBufferAllocateInfo.commandBufferCount = imagecount;
	lastResult = vkAllocateCommandBuffers(device, &vlibCommandBufferAllocateInfo, command_buffers.data());
	CHECKFAIL;

	vlibCommandBufferAllocateInfo.commandBufferCount = 1;
	lastResult = vkAllocateCommandBuffers(device, &vlibCommandBufferAllocateInfo, &SINGELTIME_COMMAND_BUFFER);
	CHECKFAIL;

	VkFenceCreateInfo fence_create_info = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
	lastResult = vkCreateFence(device, &fence_create_info, nullptr, &single_time_command_ready);
	CHECKFAIL;
}

void startSingleTimeCommand() {
	lastResult = vkResetFences(device, 1, &single_time_command_ready);
	CHECKFAIL;

	vlibCommandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vlibCommandBufferBeginInfo.pInheritanceInfo = nullptr;
	lastResult = vkBeginCommandBuffer(SINGELTIME_COMMAND_BUFFER, &vlibCommandBufferBeginInfo);
	CHECKFAIL;;
}

void endSingleTimeCommand() {
	lastResult = vkEndCommandBuffer(SINGELTIME_COMMAND_BUFFER);
	CHECKFAIL;;

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
	CHECKFAIL;

	lastResult = vkWaitForFences(device, 1, &single_time_command_ready, VK_TRUE, UINT64_MAX);
	CHECKFAIL;
}

void startupCommands() {
	startSingleTimeCommand();

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
		CHECKFAIL;;

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

		tge::gmc::loadToCommandBuffer(buffer);

		vkCmdEndRenderPass(buffer);

		lastResult = vkEndCommandBuffer(buffer);
		CHECKFAIL;
	}
}

void destroyCommandBuffer() {
	lastResult = vkDeviceWaitIdle(device);
	CHECKFAIL;
	vkFreeCommandBuffers(device, command_pool, (uint32_t)command_buffers.size(), command_buffers.data());
	vkDestroyCommandPool(device, command_pool, nullptr);
	vkDestroyFence(device, single_time_command_ready, nullptr);
}