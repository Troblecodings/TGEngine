#pragma once

#include "../Stdbase.hpp"
#include "PrePipeline.hpp"

extern VkImage depth_image;
extern VkImageView depth_image_view;
extern VkDeviceMemory depth_image_memory;

SINCE(0, 0, 4)
void createDepthTest();

SINCE(0, 0, 4)
void destroyDepthTest();