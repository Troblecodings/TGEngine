#pragma once

#include "../Stdbase.hpp"
#include "PrePipeline.hpp"
#include "Swapchain.hpp"

extern VkRenderPass renderpass;

SINCE(0, 0, 1)
void createRenderpass();

SINCE(0, 0, 1)
void destroyRenderPass();