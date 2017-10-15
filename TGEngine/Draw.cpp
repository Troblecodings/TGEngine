#include "Draw.hpp"

VkSemaphore wait_semaphore;
VkSemaphore signal_semaphore;
uint32_t* image_index;

void createSemaphores() {
	VkSemaphoreCreateInfo semaphore_create_info = {
		VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		nullptr,
		0
	};

	vkCreateSemaphore(device, &semaphore_create_info, nullptr, &wait_semaphore);
	vkCreateSemaphore(device, &semaphore_create_info, nullptr, &signal_semaphore);
}

void draw() {
	last_result = vkAcquireNextImageKHR(device, surface, 1000000, wait_semaphore, VK_NULL_HANDLE, image_index);
	HANDEL(last_result)

	VkSubmitInfo submit_info = {
		VK_STRUCTURE_TYPE_SUBMIT_INFO,
	    nullptr,
	    1,
	    &wait_semaphore,
		((VkPipelineStageFlags*) VK_PIPELINE_STAGE_ALL_COMMANDS_BIT),
	    1,
		&command_buffer,
	    1,
	    &signal_semaphore
	};

	last_result = vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE);
	HANDEL(last_result)

	VkPresentInfoKHR present_info = {
		VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
	    nullptr,
	    1,
		&signal_semaphore,
	    1,
	    &swapchain,
		image_index,
	    nullptr
	};

	last_result = vkQueuePresentKHR(queue, &present_info);
	HANDEL(last_result)
}