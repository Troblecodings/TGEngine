#pragma once

#include "TGEngine.hpp"
#include "Instance.hpp"

extern VkPhysicalDevice used_physical_device;
extern VkPhysicalDeviceProperties device_properties;
extern VkQueueFamilyProperties queue_family;
extern uint32_t queue_index;
extern VkDevice device;

void createDevice(std::vector<char*> extensions_to_enable, std::vector<char*> layers_to_enable);

void destroyDevice();