#pragma once

#include "../Stdbase.hpp"
#include "Device.hpp"
#include "RenderPass.hpp"
#include "Framebuffer.hpp"
#include "buffer/VertexBuffer.hpp"
#include "buffer/IndexBuffer.hpp"
#include "Pipe.hpp"
#include "Descriptors.hpp"
#include "../vlib/VulkanImage.hpp"
#include "buffer/StagingBuffer.hpp"
#include "../vlib/VulkanCommandBuffer.hpp"
#include "../gamecontent/Material.hpp"

#define SINGELTIME_COMMAND_BUFFER command_buffers[imagecount]

extern VkCommandPool command_pool;
extern VkFence single_time_command_ready;
extern std::vector<VkCommandBuffer> command_buffers;
extern VkDeviceSize offsets;
extern bool started;

/*
*/
void createCommandBuffer();

void startSingleTimeCommand();

void endSingleTimeCommand();

void startupCommands();

void fillCommandBuffer(IndexBuffer* ibuffer, VertexBuffer* buffer);

void destroyCommandBuffer();