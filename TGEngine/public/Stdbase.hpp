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
#include "../public/io/Properties.hpp"
#include "../public/util/Math.hpp"
#include <glm/glm.hpp>
#include "../public/Error.hpp"

extern tge::pro::Properties tgeproperties;
extern uint32_t imageCount;

#define TGE_VERSION VK_MAKE_VERSION(0, 0, 5)

#define CHECKFAIL(statement) lastResult = statement;\
if (lastResult != VK_SUCCESS) {\
TGERROR(lastResult)\
}

/*
 * This asserts a given value or statement and takes acction if the conditions are meet
 */
#define TGE_ASSERT(assertion, action) if(assertion) { action }

#if defined(_WIN32) && defined(UNICODE)
#define TGE_CRASH(message, code) MessageBox(NULL, L##message, L"Error!", MB_ICONERROR | MB_OK); TGERROR(code);
#elif defined(_WIN32)
#define TGE_CRASH(message, code) MessageBox(NULL, message, "Error!", MB_ICONERROR | MB_OK); TGERROR(code);
#else
#define TGE_CRASH(message, code)  
#endif

#ifdef DEBUG
 /*
  * This is the same as assert but with only applys to DEBUG builds
  * if the build shouldn't contain the DEBUG flag it will
  * just break down to the statement given
  */
#define TGE_ASSERT_DB(statement, assertion, action)  TGE_ASSERT(assertion, action)
#define TGE_ASSERT_NORMAL_DB(assertion, action)  TGE_ASSERT(assertion, action)
#else
#define TGE_ASSERT_DB(statement, assertion, action) statement
#define TGE_ASSERT_NORMAL_DB(assertion, action)
#endif

#define TGE_CHECK_RECREATE(statement) lastResult = statement;\
if(lastResult == VK_ERROR_OUT_OF_DATE_KHR){\
if(tge::win::isMinimized)\
return;\
recreateSwapchain();\
return;\
} else if (lastResult != VK_SUCCESS) {\
TGERROR(lastResult)\
}

#ifdef DEBUG
#define OUT_LV_DEBUG(out) std::cout << "DEBUG: " << out << std::endl;
#else
#define OUT_LV_DEBUG(out)
#endif 

