#pragma once

#include "../Stdbase.hpp"
#include "window\Window.hpp"
#include "Device.hpp"
#include "PrePipeline.hpp"
#include "Framebuffer.hpp"
#include "CommandBuffer.hpp"
#include "Pipe.hpp"
#include "RenderPass.hpp"
#include "DepthStencil.hpp"
#include "../game_content/Camera.hpp"

extern VkSwapchainKHR swapchain;
extern VkSurfaceCapabilitiesKHR surface_capabilities;

SINCE(0, 0, 1)
void createSwapchain();

SINCE(0, 0, 3)
void recreateSwapchain();

SINCE(0, 0, 1)
void destroySwapchain();