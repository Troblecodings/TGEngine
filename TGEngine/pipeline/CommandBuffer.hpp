#pragma once

#include "..\Stdbase.hpp"
#include "Device.hpp"
#include "RenderPass.hpp"
#include "Framebuffer.hpp"
#include "buffer\VertexBuffer.hpp"
#include "Pipe.hpp"
#include "Descriptors.hpp"
#include "buffer\Texturebuffer.hpp"

extern VkCommandPool command_pool;
extern std::vector<VkCommandBuffer> command_buffers;
extern VkDeviceSize offsets;
extern bool started;

SINCE(0, 0, 1)
void createCommandBuffer();

SINCE(0, 0, 2)
void singleTimeCommand();

SINCE(0, 0, 1)
void fillCommandBuffer(VertexBuffer* buffer, uint32_t index);

SINCE(0, 0, 1)
void destroyCommandBuffer();