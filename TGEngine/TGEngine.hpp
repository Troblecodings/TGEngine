#pragma once

#include <stdio.h>
#include <iostream>
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan\vulkan.h>
#include <vector>
#include <string>
#include "Annotations.hpp"
#include <thread>
#include "Properties.hpp"
#include "Math.hpp"
#include "Vertex.hpp"

extern nio::Properties properties;
extern uint32_t image_count;
#ifdef USE_INDEX_BUFFER
extern std::vector<uint32_t> indicies;
#endif

SINCE(0, 0, 2)
struct App {

	virtual void drawloop(std::vector<Vertex>* verticies) = 0;

};

SINCE(0, 0, 1)
void initTGEngine(App *app);

/*
 * Handels a vulkan error
 */
SINCE(0, 0, 1)
#define HANDEL(result)\
if (result != VK_SUCCESS) {\
std::cout << "FAILED WITH ERROR: " << result << std::endl;\
exit(result);\
}

SINCE(0, 0, 1)
UNFINISHED
#define HANDEL_RECREATE(result)\
if(result == VK_ERROR_OUT_OF_DATE_KHR){\
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
