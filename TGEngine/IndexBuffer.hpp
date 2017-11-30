#pragma once

#include "TGEngine.hpp"
#include "Device.hpp"

extern VkBuffer index_buffer;
extern VkMemoryRequirements index_buffer_requirements;
extern VkDeviceMemory index_memory;
extern uint32_t index_count;

SINCE(0, 0, 2)
void createIndexBuffer(uint32_t size);

SINCE(0, 0, 2)
void fillIndexBuffer(std::vector<uint32_t>* indicies);

SINCE(0, 0, 2)
void destroyIndexBuffer();