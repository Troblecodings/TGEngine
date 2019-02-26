#pragma once

#include "../Stdbase.hpp"
#include "Device.hpp"
#include "RenderPass.hpp"
#include "Framebuffer.hpp"
#include "buffer/VertexBuffer.hpp"
#include "buffer/IndexBuffer.hpp"
#include "Pipe.hpp"
#include "Descriptors.hpp"
#include "buffer/Texturebuffer.hpp"
#include "../vlib/VulkanImage.hpp"
#include "buffer/StagingBuffer.hpp"
#include "../vlib/VulkanCommandBuffer.hpp"

extern VkCommandPool command_pool;
extern std::vector<VkCommandBuffer> command_buffers;
extern VkDeviceSize offsets;
extern bool started;
extern uint32_t index_offset;
extern size_t vertex_offset;

SINCE(0, 0, 1)
void createCommandBuffer();

SINCE(0, 0, 4)
void startSingleTimeCommand();

SINCE(0, 0, 4)
void endSingleTimeCommand();

SINCE(0, 0, 2)
void startupCommands();

SINCE(0, 0, 1)
void fillCommandBuffer(IndexBuffer* ibuffer, VertexBuffer* buffer);

SINCE(0, 0, 1)
void destroyCommandBuffer();