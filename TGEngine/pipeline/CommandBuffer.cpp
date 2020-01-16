#include "CommandBuffer.hpp"
#include "../gamecontent/Actor.hpp"
#include "window/Window.hpp"

VkCommandPool commandPool;
VkFence singelTimeCommandBufferFence;
VkCommandBuffer* commandBuffer;
VkDeviceSize offsets = 0;

void createCommandBuffer() {
	commandBuffer = new VkCommandBuffer[(uint64_t)imagecount + 1];

	if (!commandPool) {
		VkCommandPoolCreateInfo commmandPoolCreateInfo;
		commmandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commmandPoolCreateInfo.pNext = nullptr;
		commmandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commmandPoolCreateInfo.queueFamilyIndex = queueIndex;

		CHECKFAIL(vkCreateCommandPool(device, &commmandPoolCreateInfo, nullptr, &commandPool));

		vlibCommandBufferAllocateInfo.commandPool = commandPool;
	}

	vlibCommandBufferAllocateInfo.commandBufferCount = imagecount;
	CHECKFAIL(vkAllocateCommandBuffers(device, &vlibCommandBufferAllocateInfo, commandBuffer));

	vlibCommandBufferAllocateInfo.commandBufferCount = 1;
	CHECKFAIL(vkAllocateCommandBuffers(device, &vlibCommandBufferAllocateInfo, &SINGELTIME_COMMAND_BUFFER));

	VkFenceCreateInfo fenceCreateInfo;
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = 0;
	fenceCreateInfo.pNext = nullptr;
	CHECKFAIL(vkCreateFence(device, &fenceCreateInfo, nullptr, &singelTimeCommandBufferFence));
}

void startSingleTimeCommand() {
	CHECKFAIL(vkResetFences(device, 1, &singelTimeCommandBufferFence));

	VkCommandBufferBeginInfo commandBufferBeginInfo;
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.pNext = nullptr;
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	commandBufferBeginInfo.pInheritanceInfo = nullptr;
	CHECKFAIL(vkBeginCommandBuffer(SINGELTIME_COMMAND_BUFFER, &commandBufferBeginInfo));
}

void endSingleTimeCommand() {
	CHECKFAIL(vkEndCommandBuffer(SINGELTIME_COMMAND_BUFFER));

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
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitSemaphores = VK_NULL_HANDLE;
	submitInfo.pWaitDstStageMask = VK_NULL_HANDLE;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &SINGELTIME_COMMAND_BUFFER;
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pSignalSemaphores = nullptr;

	CHECKFAIL(vkQueueSubmit(queue, 1, &submitInfo, singelTimeCommandBufferFence));

	CHECKFAIL(vkWaitForFences(device, 1, &singelTimeCommandBufferFence, VK_TRUE, UINT64_MAX));
}

void startupCommands() {
	startSingleTimeCommand();

	for each (StagingBuffer * buf in staging_buffer) {
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
	for (size_t i = 0; i < imagecount; i++) {
		VkCommandBuffer buffer = commandBuffer[i];

		VkCommandBufferInheritanceInfo command_buffer_inheritance_info = {
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
				nullptr,
				renderpass,
				0,
				frameBuffer[i],
				VK_FALSE,
				0,
				0
		};

		vlibCommandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		vlibCommandBufferBeginInfo.pInheritanceInfo = &command_buffer_inheritance_info;
		CHECKFAIL(vkBeginCommandBuffer(buffer, &vlibCommandBufferBeginInfo));

		VkRenderPassBeginInfo render_pass_begin_info = {
			VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			nullptr,
			renderpass,
			frameBuffer[i],
			{
				0,
				0,
                tge::win::mainWindowWidth,
				tge::win::mainWindowHeight
			},
			2,
			vlibClearValues
		};
		vkCmdBeginRenderPass(buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindVertexBuffers(buffer, 0, 1, &vbuffer->vertex_buffer, &offsets);

		vkCmdBindIndexBuffer(buffer, ibuffer->index_buffer, 0, VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &mainDescriptorSet[0], 0, nullptr);

		vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, tge::pip::defaultPipeline);

		tge::gmc::loadToCommandBuffer(buffer, 0);

		// Render for the UI and so on
		vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &mainDescriptorSet[1], 0, nullptr);

		tge::gmc::loadToCommandBuffer(buffer, 1);

		vkCmdEndRenderPass(buffer);

		CHECKFAIL(vkEndCommandBuffer(buffer));
	}
}

void destroyCommandBuffer() {
	CHECKFAIL(vkDeviceWaitIdle(device));
	vkFreeCommandBuffers(device, commandPool, 4, commandBuffer);
	vkDestroyCommandPool(device, commandPool, nullptr);
	vkDestroyFence(device, singelTimeCommandBufferFence, nullptr);
}