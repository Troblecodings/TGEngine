#pragma once

#include "../Stdbase.hpp"
#include "../util/VectorUtil.hpp"

extern VkResult lastResult;
extern VkInstance instance;
extern VkDebugUtilsMessengerEXT debugMessager;

SINCE(0, 0, 1)
void createInstance(std::vector<const char*> layers_to_enable, std::vector<const char*> extensions_to_enable);

/*
 * This defines the debug callback for the messages of the vulkan debug extension
 */
#ifdef DEBUG
USAGE_DEBUG
SINCE(0, 0, 4)
VkBool32 debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT                  messageType,
	const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
	void *pUserData);
#endif

SINCE(0, 0, 1)
void destroyInstance();