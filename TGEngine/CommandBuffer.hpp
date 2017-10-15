#pragma once

#include "TGEngine.hpp"
#include "Device.hpp"
#include "RenderPass.hpp"
#include "Framebuffer.hpp"
#include "Pipe.hpp"

extern VkCommandPool command_pool;
extern std::vector<VkCommandBuffer> command_buffers;

void createCommandBuffer();