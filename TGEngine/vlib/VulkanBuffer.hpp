#pragma once

#include "../Stdbase.hpp"

#define FIND_INDEX(ind, prop) for (ind = 0; ind < memory_properties.memoryTypeCount; ind++) { if ((memory_properties.memoryTypes[ind].propertyFlags & (prop)) == (prop)) break; }

// Querrys the requirements and sets the allocateInfo accordingly
// buffer -> the buffer you want to querry
// memoryIndex -> the memory index of you GPU prefiously querryed with FIND_INDEX
#define QUERRY_BUFFER_REQUIREMENTS(buffer, memoryIndex) vkGetBufferMemoryRequirements(device, buffer, &lastRequirements);\
vlib_buffer_memory_allocate_info.allocationSize = lastRequirements.size;\
vlib_buffer_memory_allocate_info.memoryTypeIndex = memoryIndex;

// Querrys the requirements and sets the allocateInfo accordingly
// image -> the image you want to querry
// memoryIndex -> the memory index of you GPU prefiously querryed with FIND_INDEX
#define QUERRY_IMAGE_REQUIREMENTS(image, memoryIndex) vkGetImageMemoryRequirements(device, image, &lastRequirements);\
vlib_buffer_memory_allocate_info.allocationSize = lastRequirements.size;\
vlib_buffer_memory_allocate_info.memoryTypeIndex = memoryIndex;


extern VkBufferCreateInfo vlib_buffer_create_info;
extern VkMemoryAllocateInfo vlib_buffer_memory_allocate_info;
extern VkBufferCopy vlib_buffer_copy;
extern uint32_t vlib_device_local_memory_index;
extern uint32_t vlib_device_host_visible_coherent_index;