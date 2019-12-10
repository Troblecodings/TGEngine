#pragma once

#include "../Stdbase.hpp"
#include "Device.hpp"
#include "window/Window.hpp"
#include "RenderPass.hpp"
#include "Swapchain.hpp"
#include "PrePipeline.hpp"

extern std::vector<VkFramebuffer> frame_buffer;
extern std::vector<VkImageView> image_view;

SINCE(0, 0, 1)
void createFramebuffer();

SINCE(0, 0, 1)
void destroyFrameBuffer();