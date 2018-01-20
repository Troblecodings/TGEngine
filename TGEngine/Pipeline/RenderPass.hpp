#pragma once

#include "..\stdbase.hpp"
#include "PrePipeline.hpp"
#include "Swapchain.hpp"

extern VkRenderPass render_pass;

SINCE(0, 0, 1)
void createRenderpass();

SINCE(0, 0, 1)
void destroyRenderPass();