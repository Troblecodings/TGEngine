#pragma once

#include "..\stdbase.hpp"
#include "Window\Window.hpp"
#include "Device.hpp"
#include "PrePipeline.hpp"

extern VkSwapchainKHR swapchain;

SINCE(0, 0, 1)
void createSwapchain();

SINCE(0, 0, 1)
void destroySwapchain();