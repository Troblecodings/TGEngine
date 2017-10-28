#pragma once

#include "Device.hpp"
#include "PrePipeline.hpp"

extern VkRenderPass render_pass;

SINCE(0,0,1)
void createRenderpass();

SINCE(0,0,1)
void destroyRenderPass();