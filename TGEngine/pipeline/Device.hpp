#pragma once

#include "Instance.hpp"
#include "window\Window.hpp"
#include "../vlib/VulkanBuffer.hpp"

extern VkPhysicalDevice used_physical_device;
extern VkPhysicalDeviceProperties device_properties;
extern VkQueueFamilyProperties queue_family;
extern uint32_t queue_index;
extern VkDevice device;
extern VkQueue queue;
extern VkPhysicalDeviceMemoryProperties memory_properties;

SINCE(0, 0, 1)
void createDevice(std::vector<char*> extensions_to_enable, std::vector<char*> layers_to_enable);

SINCE(0, 0, 1)
void destroyDevice();