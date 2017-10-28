#pragma once

#include "TGEngine.hpp"
#include "Device.hpp"

extern std::vector<VkSurfaceFormatKHR> surface_format;
extern std::vector<VkPresentModeKHR> present_mode;
extern VkSurfaceFormatKHR used_format;
extern VkPresentModeKHR used_present_mode;

void prePipeline();