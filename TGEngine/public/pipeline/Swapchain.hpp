#pragma once

#include "../../public/Stdbase.hpp"
#include "../../public/pipeline/Device.hpp"
#include "../../public/pipeline/PrePipeline.hpp"
#include "../../public/pipeline/Framebuffer.hpp"
#include "../../public/pipeline/CommandBuffer.hpp"
#include "../../public/pipeline/Pipe.hpp"
#include "../../public/pipeline/RenderPass.hpp"
#include "../../public/pipeline/DepthStencil.hpp"
#include "../../public/gamecontent/camera/Camera.hpp"

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
