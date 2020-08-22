#pragma once

#include "../../../public/Stdbase.hpp"
#include "../../../public/vlib/VulkanBuffer.hpp"
#include "../../../public/pipeline/Device.hpp"
#include "../../../public/pipeline/buffer/Memory.hpp"

/*
 * Holds information to create a staging buffer.
 * A staging buffer is a transfer buffer.
 * It transfers data from a host visible memory
 * onto device local memory
 */
struct StagingBuffer {
	VkDeviceSize   size;
	VkBuffer*      destination;
	VkBuffer       staging_buffer;
	VkDeviceMemory staging_buffer_device_memory;
};

extern std::vector<StagingBuffer*> staging_buffer;

void createStagingBuffer(StagingBuffer* staging_buffer);

void destroyStagingBuffer();
