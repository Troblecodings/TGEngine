#pragma once

#include "stdbase.hpp"
#include "Device.hpp"

extern std::vector<VkSurfaceFormatKHR> surface_format;
extern std::vector<VkPresentModeKHR> present_mode;
extern VkSurfaceFormatKHR used_format;
extern VkPresentModeKHR used_present_mode;

SINCE(0, 0, 1)
void prePipeline();