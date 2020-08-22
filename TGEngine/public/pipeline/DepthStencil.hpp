#pragma once

#include "../Stdbase.hpp"
#include "PrePipeline.hpp"
#include "../vlib/VulkanBuffer.hpp"

extern VkImage depthImage;
extern VkImageView depthImageView;
extern VkDeviceMemory depthImageMemory;

void createDepthTest();

void destroyDepthTest();