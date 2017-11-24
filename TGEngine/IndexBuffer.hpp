#pragma once

#include "TGEngine.hpp"
#include "Device.hpp"

SINCE(0, 0, 2)
void createIndexBuffer(uint32_t size);

SINCE(0, 0, 2)
void fillIndexBuffer(std::vector<uint32_t> indicies);

SINCE(0, 0, 2)
void destroyIndexBuffer();