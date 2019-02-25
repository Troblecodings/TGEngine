#pragma once

#include "../Stdbase.hpp"

#define FIND_INDEX(ind, prop) for (ind = 0; ind < memory_properties.memoryTypeCount; ind++) { if ((memory_properties.memoryTypes[ind].propertyFlags & (prop)) == (prop)) break; }

extern VkBufferCreateInfo vlib_buffer_create_info;
extern VkMemoryAllocateInfo vlib_buffer_memory_allocate_info;
extern VkBufferCopy vlib_buffer_copy;
extern uint32_t vlib_device_local_memory_index;
extern uint32_t vlib_device_host_visible_coherent_index;