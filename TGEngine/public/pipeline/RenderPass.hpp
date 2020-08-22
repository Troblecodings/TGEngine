#pragma once

#include "../../public/Stdbase.hpp"
#include "../../public/pipeline/PrePipeline.hpp"
#include "../../public/pipeline/Swapchain.hpp"

extern VkRenderPass renderPass;

void createRenderpass();

void destroyRenderPass();