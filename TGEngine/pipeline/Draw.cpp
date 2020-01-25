#include "Draw.hpp"
#include "Swapchain.hpp"
#include "window/Window.hpp"

uint32_t imageIndex;
VkFence drawFence;

void createMutex() {
	VkFenceCreateInfo fenceCreateInfo;
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.pNext = nullptr;
	fenceCreateInfo.flags = 0;
	vkCreateFence(device, &fenceCreateInfo, nullptr, &drawFence);
}

void startdraw() {
	CHECKFAIL(vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, VK_NULL_HANDLE, drawFence, &imageIndex))
	TGE_CHECK_RECREATE;

	CHECKFAIL(vkWaitForFences(device, 1, &drawFence, VK_TRUE, UINT64_MAX));
	CHECKFAIL(vkResetFences(device, 1, &drawFence));
}

void submit() {
	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitSemaphores = VK_NULL_HANDLE;
	submitInfo.pWaitDstStageMask = VK_NULL_HANDLE;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer[imageIndex];
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pSignalSemaphores = nullptr;

	CHECKFAIL(vkQueueSubmit(queue, 1, &submitInfo, drawFence))
	TGE_CHECK_RECREATE;
}

void present() {
	CHECKFAIL(vkWaitForFences(device, 1, &drawFence, VK_TRUE, UINT64_MAX));
	CHECKFAIL(vkResetFences(device, 1, &drawFence));

	VkPresentInfoKHR presentInfoKHR;
	presentInfoKHR.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfoKHR.pNext = nullptr;
	presentInfoKHR.waitSemaphoreCount = 0;
	presentInfoKHR.pWaitSemaphores = nullptr;
	presentInfoKHR.swapchainCount = 1;
	presentInfoKHR.pSwapchains = &swapchain;
	presentInfoKHR.pImageIndices = &imageIndex;
	presentInfoKHR.pResults = nullptr;

	CHECKFAIL(vkQueuePresentKHR(queue, &presentInfoKHR))
	TGE_CHECK_RECREATE;
}

void destroyMutex() {
	CHECKFAIL(vkDeviceWaitIdle(device));

	vkDestroyFence(device, drawFence, nullptr);
}