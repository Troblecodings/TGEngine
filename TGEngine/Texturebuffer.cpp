#include "Texturebuffer.hpp"

void createTexturebuffer(uint32_t size) {
	
	VkBufferCreateInfo buffer_create_info = {
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
	    nullptr,
	    0,
	    size,
	//VK_BUFFER
    //sharingMode;
	//queueFamilyIndexCount;
	//pQueueFamilyIndices;
	};
}