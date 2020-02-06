#pragma once

#include "../Stdbase.hpp"
#include "Device.hpp"
#include <algorithm>

extern VkSurfaceFormatKHR usedSurfaceFormat;
extern VkFormat usedDepthFormat;
extern VkPresentModeKHR usedPresentMode;
extern VkSampleCountFlagBits usedSampleFlag;

void prePipeline();