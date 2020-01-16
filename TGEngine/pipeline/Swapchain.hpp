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
extern std::vector<VkImage> swapchain_images;
extern VkSurfaceCapabilitiesKHR surface_capabilities;
extern VkImage color_image;
extern VkImageView color_image_view;
extern VkDeviceMemory color_image_memory;

void createSwapchain();

void createColorResouce();

void destroyColorResouce();

void recreateSwapchain();

void destroySwapchain();