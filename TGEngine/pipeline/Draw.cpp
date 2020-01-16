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
	CHECKFAIL(vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, VK_NULL_HANDLE, fence, &image_index))
	TGE_CHECK_RECREATE;

	CHECKFAIL(vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX));
	CHECKFAIL(vkResetFences(device, 1, &fence));
}

void submit(IndexBuffer* ibuffer, VertexBuffer* vbuffer) {
	VkSubmitInfo submit_info = {
		VK_STRUCTURE_TYPE_SUBMIT_INFO,
		nullptr,
		0,
		nullptr,
		nullptr,
		1,
		&commandBuffer[image_index],
		0,
		nullptr
	};
	CHECKFAIL(vkQueueSubmit(queue, 1, &submit_info, fence))
	TGE_CHECK_RECREATE;
}

void present(IndexBuffer* ibuffer, VertexBuffer* vbuffer) {
	CHECKFAIL(vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX));
	CHECKFAIL(vkResetFences(device, 1, &fence));
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
	CHECKFAIL(vkQueuePresentKHR(queue, &present_info))
	TGE_CHECK_RECREATE;
}

void destroySemaphores() {
	CHECKFAIL(vkDeviceWaitIdle(device));

	vkDestroyFence(device, fence, nullptr);
}