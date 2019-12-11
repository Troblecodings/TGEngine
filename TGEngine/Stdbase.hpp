#pragma once

#include <stdio.h>
#include <iostream>
#include <cstddef>
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#undef VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <thread>
#include "io/Properties.hpp"
#include "util/Math.hpp"
#include <glm/glm.hpp>
#include "util/TGVertex.hpp"
#include "Error.hpp"

extern prop::Properties* properties;
extern uint32_t imagecount;

#define TGE_VERSION VK_MAKE_VERSION(0, 0, 5)

#define CHECKFAIL(statement) lastResult = statement;\
if (lastResult != VK_SUCCESS) {\
TGERROR(lastResult)\
}

/*
 * This asserts a given value or statement and takes acction if the conditions are meet
 */
#define TGE_ASSERT(assertion, action) if(assertion) { action }

#ifdef DEBUG
 /*
  * This is the same as assert but with only applys to DEBUG builds
  * if the build shouldn't contain the DEBUG flag it will
  * just break down to the statement given
  */
#define TGE_ASSERT_DB(statement, assertion, action)  TGE_ASSERT(statement assertion, action)
#else
#define TGE_ASSERT_DB(statement, assertion, action) statement
#endif

#define TGE_CHECK_RECREATE else if(lastResult == VK_ERROR_OUT_OF_DATE_KHR){\
if(windowList[0]->minimized){\
return;\
}\
recreateSwapchain(ibuffer, vbuffer);\
}

#ifdef DEBUG
#define OUT_LV_DEBUG(out) std::cout << "DEBUG: " << out << std::endl;
#else
#define OUT_LV_DEBUG(out)
#endif 

