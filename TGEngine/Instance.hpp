#pragma once

#include "TGEngine.hpp"
#include "Properties.hpp"

extern VkResult last_result;
extern VkInstance instance;
extern VkAllocationCallbacks allocator;

void createInstance(nio::Properties propertys, std::vector<char*> layers_to_enable, std::vector<char*> extensions_to_enable);

void destroyInstance();