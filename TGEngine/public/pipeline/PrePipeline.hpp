#pragma once

#include "../../public/Stdbase.hpp"
#include "../../public/pipeline/Device.hpp"
#include <algorithm>

extern VkSurfaceFormatKHR usedSurfaceFormat;
extern VkFormat usedDepthFormat;
extern VkPresentModeKHR usedPresentMode;
extern VkSampleCountFlagBits usedSampleFlag;

void prePipeline();