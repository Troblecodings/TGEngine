#pragma once

#include "App.hpp"

extern nio::Properties properties;
extern uint32_t image_count;
#ifdef USE_INDEX_BUFFER
extern std::vector<uint32_t> indicies;
#endif

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
