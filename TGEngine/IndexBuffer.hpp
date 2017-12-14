#pragma once

#include "Memory.hpp"

extern VkBuffer index_buffer;
extern uint32_t index_count;
extern uint32_t index_buffer_index;

SINCE(0, 0, 2)
void createIndexBuffer(uint32_t size);

SINCE(0, 0, 2)
void fillIndexBuffer(std::vector<uint32_t>* indicies);