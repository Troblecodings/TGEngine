#pragma once

#include "../Stdbase.hpp"
#include "Device.hpp"
#include "RenderPass.hpp"
#include "Framebuffer.hpp"
#include "buffer/VertexBuffer.hpp"
#include "buffer/IndexBuffer.hpp"
#include "Pipe.hpp"
#include "Descriptors.hpp"
#include "buffer/StagingBuffer.hpp"
#include "../vlib/VulkanCommandBuffer.hpp"
#include "../gamecontent/Material.hpp"

#define SINGLE_TIME_COMMAND_BUFFER commandBuffer[imageCount]

extern VkCommandPool commandPool;
extern VkFence singleTimeCommandBufferFence;
extern VkCommandBuffer* commandBuffer;

void createCommandBuffer();

void startSingleTimeCommand();

void endSingleTimeCommand();

void startupCommands();

void fillCommandBuffer();

void destroyCommandBuffer();
