#pragma once

#include "../../public/Stdbase.hpp"

#define FIND_INDEX(ind, prop) for (ind = 0; ind < memoryProperties.memoryTypeCount; ind++) { if ((memoryProperties.memoryTypes[ind].propertyFlags & (prop)) == (prop)) break; }

// Querrys the requirements and sets the allocateInfo accordingly
// buffer -> the buffer you want to querry
// memoryIndex -> the memory index of you GPU prefiously querryed with FIND_INDEX
#define QUERRY_BUFFER_REQUIREMENTS(buffer, memoryIndex) vkGetBufferMemoryRequirements(device, buffer, &lastRequirements);\
vlibBufferMemoryAllocateInfo.allocationSize = lastRequirements.size;\
vlibBufferMemoryAllocateInfo.memoryTypeIndex = memoryIndex;

// Querrys the requirements and sets the allocateInfo accordingly
// image -> the image you want to querry
// memoryIndex -> the memory index of you GPU prefiously querryed with FIND_INDEX
#define QUERRY_IMAGE_REQUIREMENTS(image, memoryIndex) vkGetImageMemoryRequirements(device, image, &lastRequirements);\
vlibBufferMemoryAllocateInfo.allocationSize = lastRequirements.size;\
vlibBufferMemoryAllocateInfo.memoryTypeIndex = memoryIndex;


extern VkBufferCreateInfo vlibBufferCreateInfo;
extern VkMemoryAllocateInfo vlibBufferMemoryAllocateInfo;
extern VkBufferCopy vlibBufferCopy;
extern uint32_t vlibDeviceLocalMemoryIndex;
extern uint32_t vlibDeviceHostVisibleCoherentIndex;

inline VkBufferCreateInfo bufferCreateInfo(size_t size, VkBufferUsageFlags flags) {
	VkBufferCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.size = size;
	createInfo.usage = flags;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.queueFamilyIndexCount = 0;
	createInfo.pQueueFamilyIndices = nullptr;
	return createInfo;
}