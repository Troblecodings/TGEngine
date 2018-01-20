#pragma once

#include "..\stdbase.hpp"
#include "Device.hpp"
#include "RenderPass.hpp"
#include "Framebuffer.hpp"
#include "Buffer\VertexBuffer.hpp"
#include "Pipe.hpp"
#include "Descriptors.hpp"

extern VkCommandPool command_pool;
extern std::vector<VkCommandBuffer> command_buffers;

SINCE(0, 0, 1)
void createCommandBuffer();

SINCE(0, 0, 1)
void fillCommandBuffer(VertexBuffer* buffer);

SINCE(0, 0, 1)
void destroyCommandBuffer();