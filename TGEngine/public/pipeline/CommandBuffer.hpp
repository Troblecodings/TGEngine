#pragma once

#include "../../public/Stdbase.hpp"
#include "../../public/pipeline/Device.hpp"
#include "../../public/pipeline/RenderPass.hpp"
#include "../../public/pipeline/Framebuffer.hpp"
#include "../../public/pipeline/Pipe.hpp"
#include "../../public/pipeline/Descriptors.hpp"
#include "../../public/pipeline/buffer/StagingBuffer.hpp"
#include "../../public/vlib/VulkanCommandBuffer.hpp"
#include "../../public/gamecontent/Material.hpp"

#define SINGLE_TIME_COMMAND_BUFFER commandBuffer[imageCount]

constexpr uint32_t LAYER_ID_OFFSET = 4;

extern VkCommandPool commandPool;
extern VkFence singleTimeCommandBufferFence;
extern VkCommandBuffer* commandBuffer;

void createCommandBuffer();

void startSingleTimeCommand();

void endSingleTimeCommand();

void fillCommandBuffer();

void destroyCommandBuffer();
