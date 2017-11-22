#include "Draw.hpp"

VkSemaphore wait_semaphore;
VkSemaphore signal_semaphore;
uint32_t image_index;
std::vector<VkPipelineStageFlags> stage_flags = {VK_PIPELINE_STAGE_ALL_COMMANDS_BIT};

void createSemaphores() {
	VkSemaphoreCreateInfo semaphore_create_info = {
		VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		nullptr,
		0
	};

	vkCreateSemaphore(device, &semaphore_create_info, nullptr, &wait_semaphore);
	vkCreateSemaphore(device, &semaphore_create_info, nullptr, &signal_semaphore);
}

void draw(std::vector<Vertex> vrt) {
	last_result = vkAcquireNextImageKHR(device, swapchain, 1000000, wait_semaphore, VK_NULL_HANDLE, &image_index);
	HANDEL_RECREATE(last_result)

	if (vrt.size() != vertex_count) {
		vkDeviceWaitIdle(device);
		fillVertexBuffer(vrt);
		fillCommandBuffer();
	}

	VkSubmitInfo submit_info = {
		VK_STRUCTURE_TYPE_SUBMIT_INFO,
	    nullptr,
	    1,
	    &wait_semaphore,
		stage_flags.data(),
	    1,
		&command_buffers[image_index],
	    1,
	    &signal_semaphore
	};

	last_result = vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE);
	HANDEL_RECREATE(last_result)

	VkPresentInfoKHR present_info = {
		VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
	    nullptr,
	    1,
		&signal_semaphore,
	    1,
	    &swapchain,
		&image_index,
	    nullptr
	};

	last_result = vkQueuePresentKHR(queue, &present_info);
	HANDEL_RECREATE(last_result);
}

void destroySemaphores() {
	last_result = vkDeviceWaitIdle(device);
	HANDEL(last_result)

	vkDestroySemaphore(device, wait_semaphore, nullptr);
	vkDestroySemaphore(device, signal_semaphore, nullptr);
}