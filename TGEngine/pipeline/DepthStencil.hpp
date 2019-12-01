#pragma once

#include "../Stdbase.hpp"
#include "PrePipeline.hpp"
#include "../vlib/VulkanBuffer.hpp"
#include "../vlib/VulkanImage.hpp"

extern VkImage depth_image;
extern VkImageView depth_image_view;
extern VkDeviceMemory depth_image_memory;

void createDepthTest();

void destroyDepthTest();