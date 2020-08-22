#pragma once

#include "../../public/Stdbase.hpp"
#include "../../public/pipeline/PrePipeline.hpp"
#include "../../public/vlib/VulkanBuffer.hpp"

extern VkImage depthImage;
extern VkImageView depthImageView;
extern VkDeviceMemory depthImageMemory;

void createDepthTest();

void destroyDepthTest();