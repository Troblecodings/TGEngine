#pragma once

#include "../Stdbase.hpp"
#include "Device.hpp"
#include <algorithm>

extern std::vector<VkSurfaceFormatKHR> surface_format;
extern std::vector<VkPresentModeKHR> present_mode;
extern VkSurfaceFormatKHR used_format;
extern VkFormat used_depth_format;
extern VkPresentModeKHR used_present_mode;
extern VkSampleCountFlagBits usedMSAAFlag;

void prePipeline();