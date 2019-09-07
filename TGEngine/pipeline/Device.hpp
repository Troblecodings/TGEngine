#pragma once

#include "Instance.hpp"
#include "../vlib/VulkanBuffer.hpp"

extern VkPhysicalDevice physicalDevice;
extern VkPhysicalDeviceProperties deviceProperties;
extern VkQueueFamilyProperties queueFamily;
extern uint32_t queueIndex;
extern VkDevice device;
extern VkQueue queue;
extern VkPhysicalDeviceMemoryProperties memoryProperties;

SINCE(0, 0, 1)
void createDevice();

SINCE(0, 0, 1)
void destroyDevice();