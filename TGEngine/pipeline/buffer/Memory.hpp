#pragma once

#include "../../pipeline/Device.hpp"
#include "../../vlib/VulkanBuffer.hpp"

extern std::vector<VkBuffer> buffers;
extern std::vector<VkDeviceSize> buffer_sizes;
extern std::vector<VkDeviceSize> buffer_offsets;
extern VkDeviceMemory device_memory;
extern VkMemoryRequirements last_requirements;
extern VkDeviceSize _impl_size;

/*
 * Adds a buffer to the creation list
 */
SINCE(0, 0, 4)
uint32_t addBuffer(VkBuffer buffer);

SINCE(0, 0, 2)
void allocateAllBuffers();

SINCE(0, 0, 2)
void mapMemory(uint32_t buffer_index, void** mapped_memory);

SINCE(0, 0, 2)
void unmapMemory();

SINCE(0, 0, 2)
void destroyMemory();
