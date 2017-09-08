#pragma once

#include "stdbase.h"
#include <time.h>
#include <chrono>

SINCE(0, 0, 7)
#define printExtend(x) cout << "width:" << x.width << " height:" << x.height << endl; 

SINCE(0, 0, 7)
#define printOffset(v) cout << "x:" << v.x << " y:" << v.y << endl; 

/* 
 * Prints each feature of the prameter VkPhysicalDeviceFeatures device_features
 */
SINCE(0, 0, 6)
DEPRECATED
void printFeatures(VkPhysicalDeviceFeatures device_features);

/*
 * WTF it prints the time dude why you're reading this?
 */
SINCE(0, 0, 6)
DEPRECATED
void printTime();

/*
 * Prints a version with the Format MAJOR.MINOR.VERSION e.g. 1.0.0
 * You can built it with VK_MAKE_VERSION
 */
SINCE(0, 0, 6)
void printVersion(int version);

/*
 * Handels a vulkan error
 */
SINCE(0, 0, 6)
void handel(VkResult result);

/*
 * Throws a critical error 
 */
SINCE(0, 0, 6)
void error(char* errorname, int errorcode);

SINCE(0, 0, 6)
DEPRECATED
void debugLayerAndExtensions();