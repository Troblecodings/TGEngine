#pragma once

#include "Memory.hpp"

SINCE(0, 0, 2)
uint32_t createUniformBuffer(uint32_t size);

SINCE(0, 0, 2)
void fillUniformBuffer(uint32_t index, void* input, uint32_t size);