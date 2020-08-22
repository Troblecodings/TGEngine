#pragma once

#include "../../public/Stdbase.hpp"
#include "../../public/util/VectorUtil.hpp"

extern VkResult lastResult;
extern VkInstance instance;
extern VkDebugUtilsMessengerEXT debugMessager;

void createInstance();

#ifdef DEBUG
/*
 * This defines the debug callback for the messages of the Vulkan debug extension
 */
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT                  messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData);
#endif

void destroyInstance();
