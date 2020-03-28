#include "CommandBuffer.hpp"
#include "../gamecontent/Actor.hpp"
#include "window/Window.hpp"
#include "../io/Resource.hpp"

VkCommandPool commandPool;
VkFence singelTimeCommandBufferFence;
VkCommandBuffer* commandBuffer;

void createCommandBuffer() {
	commandBuffer = new VkCommandBuffer[(uint64_t)imageCount + 1];

	if (!commandPool) {
		VkCommandPoolCreateInfo commmandPoolCreateInfo;
		commmandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commmandPoolCreateInfo.pNext = nullptr;
		commmandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commmandPoolCreateInfo.queueFamilyIndex = queueIndex;

		CHECKFAIL(vkCreateCommandPool(device, &commmandPoolCreateInfo, nullptr, &commandPool));
	}

	VkCommandBufferAllocateInfo commandBufferAllocateInfo;
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.pNext = nullptr;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = imageCount + 1;
	CHECKFAIL(vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, commandBuffer));

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
	CHECKFAIL(vkBeginCommandBuffer(SINGLE_TIME_COMMAND_BUFFER, &commandBufferBeginInfo));
}

void endSingleTimeCommand() {
	CHECKFAIL(vkEndCommandBuffer(SINGLE_TIME_COMMAND_BUFFER));

	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitSemaphores = VK_NULL_HANDLE;
	submitInfo.pWaitDstStageMask = VK_NULL_HANDLE;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &SINGLE_TIME_COMMAND_BUFFER;
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pSignalSemaphores = nullptr;

	CHECKFAIL(vkQueueSubmit(queue, 1, &submitInfo, singelTimeCommandBufferFence));

	CHECKFAIL(vkWaitForFences(device, 1, &singelTimeCommandBufferFence, VK_TRUE, UINT64_MAX));
}

void fillCommandBuffer() {
	for (size_t i = 0; i < imageCount; i++) {
		VkCommandBuffer buffer = commandBuffer[i];

		VkCommandBufferInheritanceInfo commandBufferInheritanceInfo;
		commandBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		commandBufferInheritanceInfo.pNext = nullptr;
		commandBufferInheritanceInfo.renderPass = renderPass;
		commandBufferInheritanceInfo.subpass = 0;
		commandBufferInheritanceInfo.framebuffer = frameBuffer[i];
		commandBufferInheritanceInfo.occlusionQueryEnable = VK_FALSE;
		commandBufferInheritanceInfo.queryFlags = 0;
		commandBufferInheritanceInfo.pipelineStatistics = 0;

		VkCommandBufferBeginInfo commandBufferBeginInfo;
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.pNext = nullptr;
		commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		commandBufferBeginInfo.pInheritanceInfo = &commandBufferInheritanceInfo;
		CHECKFAIL(vkBeginCommandBuffer(buffer, &commandBufferBeginInfo));

		VkClearValue clearValues[2];
		clearValues[0] = { 1, 1, 1, 1 };
		clearValues[1] = { 1.0f, 0 };

		VkRenderPassBeginInfo renderPassBeginInfo;
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.pNext = nullptr;
		renderPassBeginInfo.renderPass = renderPass;
		renderPassBeginInfo.framebuffer = frameBuffer[i];
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent.width = tge::win::mainWindowWidth;
		renderPassBeginInfo.renderArea.extent.height = tge::win::mainWindowHeight;
		renderPassBeginInfo.clearValueCount = 2;
		renderPassBeginInfo.pClearValues = clearValues;
		vkCmdBeginRenderPass(buffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkDeviceSize offsets = 0;
		vkCmdBindVertexBuffers(buffer, 0, 1, &tge::io::currentMap.mapBuffers[1].buffer, &offsets);

		vkCmdBindIndexBuffer(buffer, tge::io::currentMap.mapBuffers[0].buffer, 0, VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 2, mainDescriptorSets, 0, nullptr);

		vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, tge::pip::defaultPipeline[0]);

		tge::gmc::loadToCommandBuffer(buffer, 0);

		// Render for the UI and so on
		vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 2, new VkDescriptorSet[2]{ mainDescriptorSets[0], mainDescriptorSets[2] }, 0, nullptr);

		tge::gmc::loadToCommandBuffer(buffer, 1);

		// Binding for 
		vkCmdBindVertexBuffers(buffer, 1, 1, &vertexBuffer.vertex_buffer, &offsets);

		//Instance based rendering for UI
		vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, tge::pip::defaultPipeline[1]);

		tge::gmc::loadToCommandBuffer(buffer, 2);

		//Instance based rendering for the game layer
		vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 2, mainDescriptorSets, 0, nullptr);

		tge::gmc::loadToCommandBuffer(buffer, 3);

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