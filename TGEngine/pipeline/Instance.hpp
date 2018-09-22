#pragma once

#include "..\Stdbase.hpp"

extern VkResult last_result;
extern VkInstance instance;

SINCE(0, 0, 1)
void createInstance(std::vector<const char*> layers_to_enable, std::vector<const char*> extensions_to_enable);

SINCE(0, 0, 1)
void destroyInstance();