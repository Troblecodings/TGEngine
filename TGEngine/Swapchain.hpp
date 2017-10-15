#pragma once

#include "TGEngine.hpp"
#include "Window.hpp"
#include "Device.hpp"

extern VkSwapchainKHR swapchain;

SINCE(0, 0, 1)
void createSwapchain();

SINCE(0, 0, 1)
void destroySwapchain();