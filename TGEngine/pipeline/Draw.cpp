#include "Draw.hpp"

uint32_t image_index;
VkFence fence;

void createSemaphores() {
	VkFenceCreateInfo info = {
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		nullptr,
		0
	};
	vkCreateFence(device, &info, nullptr, &fence);
}

void startdraw(IndexBuffer* ibuffer, VertexBuffer* vbuffer) {
	lastResult = vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, VK_NULL_HANDLE, fence, &image_index);
	HANDEL_RECREATE(lastResult)
		lastResult = vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
	CHECKFAIL;
		lastResult = vkResetFences(device, 1, &fence);
	CHECKFAIL;
}

void submit(IndexBuffer* ibuffer, VertexBuffer* vbuffer) {
	VkSubmitInfo submit_info = {
		VK_STRUCTURE_TYPE_SUBMIT_INFO,
		nullptr,
		0,
		nullptr,
		nullptr,
		1,
		&command_buffers[image_index],
		0,
		nullptr
	};
	lastResult = vkQueueSubmit(queue, 1, &submit_info, fence);
	HANDEL_RECREATE(lastResult)
}

void present(IndexBuffer* ibuffer, VertexBuffer* vbuffer) {
	lastResult = vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
	CHECKFAIL;
		lastResult = vkResetFences(device, 1, &fence);
	CHECKFAIL;
		VkPresentInfoKHR present_info = {
			VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			nullptr,
			0,
			nullptr,
			1,
			&swapchain,
			&image_index,
			nullptr
	};
	lastResult = vkQueuePresentKHR(queue, &present_info);
	HANDEL_RECREATE(lastResult);
}

void destroySemaphores() {
	lastResult = vkDeviceWaitIdle(device);
	CHECKFAIL;

		vkDestroyFence(device, fence, nullptr);
}