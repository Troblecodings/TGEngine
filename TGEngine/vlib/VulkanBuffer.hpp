#pragma once

#include "../Stdbase.hpp"

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