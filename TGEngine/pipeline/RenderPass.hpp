#pragma once

#include "../Stdbase.hpp"
#include "PrePipeline.hpp"
#include "Swapchain.hpp"

extern VkRenderPass renderpass;

void createRenderpass();

void destroyRenderPass();