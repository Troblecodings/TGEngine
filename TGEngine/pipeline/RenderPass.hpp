#pragma once

#include "../Stdbase.hpp"
#include "PrePipeline.hpp"
#include "Swapchain.hpp"

extern VkRenderPass renderPass;

void createRenderpass();

void destroyRenderPass();