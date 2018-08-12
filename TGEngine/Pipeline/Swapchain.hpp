#pragma once

#include "..\stdbase.hpp"
#include "Window\Window.hpp"
#include "Device.hpp"
#include "PrePipeline.hpp"
#include "Framebuffer.hpp"
#include "CommandBuffer.hpp"
#include "Pipe.hpp"
#include "RenderPass.hpp"

extern VkSwapchainKHR swapchain;

SINCE(0, 0, 1)
void createSwapchain();

SINCE(0, 0, 3)
void recreateSwapchain();

SINCE(0, 0, 1)
void destroySwapchain();