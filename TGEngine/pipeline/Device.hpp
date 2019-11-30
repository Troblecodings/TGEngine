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

void createDevice();

void destroyDevice();