#pragma once

#include "../Stdbase.hpp"

#define FIND_INDEX(ind, prop) for (ind = 0; ind < memory_properties.memoryTypeCount; ind++) { if ((memory_properties.memoryTypes[ind].propertyFlags & (prop)) == (prop)) break; }

extern VkBufferCreateInfo vlib_buffer_create_info;