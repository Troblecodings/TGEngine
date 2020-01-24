#pragma once

#include "../Stdbase.hpp"
#include "Device.hpp"
#include "PrePipeline.hpp"
#include "Framebuffer.hpp"
#include "CommandBuffer.hpp"
#include "Pipe.hpp"
#include "RenderPass.hpp"
#include "DepthStencil.hpp"
#include "../gamecontent/camera/Camera.hpp"
#include "buffer/IndexBuffer.hpp"
#include "buffer/VertexBuffer.hpp"
#include "../ui/UIEntity.hpp"

extern VkSwapchainKHR swapchain;
extern VkImage* swapchainImages;
/*
 * Contains the supported image usage flags,
 * the min and max image count, as well as
 * min and max extent
 */
extern VkSurfaceCapabilitiesKHR surfaceCapabilities;
extern VkImage colorImage;
extern VkImageView colorImageView;
extern VkDeviceMemory colorImageMemory;

void createSwapchain();

void createColorResouce();

void destroyColorResouce();

void recreateSwapchain();

void destroySwapchain();
