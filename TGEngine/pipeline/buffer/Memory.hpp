#pragma once

#include "..\..\pipeline\Device.hpp"

extern std::vector<VkBuffer> buffers;
extern std::vector<bool> isImage;
extern std::vector<VkDeviceSize> buffer_sizes;
extern std::vector<VkDeviceSize> buffer_offsets;
extern VkDeviceMemory device_memory;

extern uint32_t index;
extern VkDeviceSize _impl_size;
extern uint32_t memory_flags;

SINCE(0, 0, 2)
uint32_t getMemoryRequirements(VkBuffer buffer, uint32_t memoryflags);

SINCE(0, 0, 2)
void allocateAllBuffers();

SINCE(0, 0, 2)
void mapMemory(uint32_t buffer_index, void** mapped_memory);

SINCE(0, 0, 2)
void unmapMemory();

SINCE(0, 0, 2)
void destroyMemory();
