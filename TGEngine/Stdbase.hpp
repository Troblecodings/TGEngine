#pragma once

#include <stdio.h>
#include <iostream>
#include <cstddef>
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "vulkan/vulkan.hpp"
#include <vector>
#include <string>
#include "util/Annotations.hpp"
#include <thread>
#include "io/Properties.hpp"
#include "util/Math.hpp"
#include <glm/glm.hpp>
#include "util/TGVertex.hpp"
#include "Error.hpp"

extern prop::Properties* properties;
extern uint32_t image_count;

SINCE(0, 0, 4)
#define TGE_VERSION VK_MAKE_VERSION(0, 0, 4)

SINCE(0, 0, 1)
#define HANDEL(result)\
if (result != VK_SUCCESS) {\
std::cout << "FAILED WITH ERROR: " << result << std::endl;\
exit(result);\
}

SINCE(0, 0, 1)
#define HANDEL_RECREATE(result)\
if(result == VK_ERROR_OUT_OF_DATE_KHR){\
if(window_list[0]->minimized){\
return;\
}\
recreateSwapchain();\
}else if(result != VK_SUCCESS){\
HANDEL(result)\
}

SINCE(0, 0, 1)
USAGE_DEBUG
#ifdef DEBUG
#define OUT_LV_DEBUG(out) std::cout << "DEBUG: " << out << std::endl;
#else
#define OUT_LV_DEBUG(out)
#endif 

