#pragma once

#include "..\stdbase.hpp"
#include "Device.hpp"
#include "RenderPass.hpp"
#include "Framebuffer.hpp"
#include "Buffer\VertexBuffer.hpp"
#include "Pipe.hpp"
#include "Descriptors.hpp"
#include "Buffer\Texturebuffer.hpp"

extern VkCommandPool command_pool;
extern std::vector<VkCommandBuffer> command_buffers;
extern std::vector<VkDeviceSize> offsets;

SINCE(0, 0, 1)
void createCommandBuffer();

SINCE(0, 0, 1)
void singleTimeCommand();

SINCE(0, 0, 1)
void fillCommandBuffer(VertexBuffer* buffer);

SINCE(0, 0, 1)
void destroyCommandBuffer();