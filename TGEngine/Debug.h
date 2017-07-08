#pragma once

#include "stdbase.h"
#include <time.h>
#include <chrono>

/* 
 * Prints each feature of the prameter VkPhysicalDeviceFeatures device_features
 */
void printFeatures(VkPhysicalDeviceFeatures device_features);

/*
 * WTF it prints the time dude why you're reading this?
 */
void printTime();

/*
 * Prints a version with the Format MAJOR.MINOR.VERSION e.g. 1.0.0
 * You can built it with VK_MAKE_VERSION
 */
void printVersion(int version);

/*
 * Handels a vulkan error
 */
void handel(VkResult result);

/*
 * Throws a critical error 
 */
void error(char* errorname, int errorcode);