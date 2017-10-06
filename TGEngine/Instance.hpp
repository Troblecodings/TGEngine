#pragma once

#include "TGEngine.hpp"
#include "Properties.hpp"

extern VkResult last_result;
extern VkInstance instance;
extern VkAllocationCallbacks allocator;

SINCE(0, 0, 1)
void createInstance(nio::Properties propertys, std::vector<const char*> layers_to_enable, std::vector<const char*> extensions_to_enable);

SINCE(0, 0, 1)
void destroyInstance();