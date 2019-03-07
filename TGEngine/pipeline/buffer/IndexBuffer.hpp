#pragma once

#include "Memory.hpp"
#include "StagingBuffer.hpp"

struct IndexBuffer{
	INPUT
	uint32_t size;

	OUTPUT
	VkBuffer index_buffer;
	VkDeviceMemory device_memory;
	uint32_t max_size;
	uint32_t index_count;
	StagingBuffer stag_buf;
	void* memory;

	SINCE(0, 0, 2)
	void addIndex(uint32_t index);
	
	SINCE(0, 0, 4)
	void start();

	SINCE(0, 0, 4)
	void end();
};

extern uint32_t index_offset;

SINCE(0, 0, 2)
void createIndexBuffer(IndexBuffer* buffer);

SINCE(0, 0, 2)
void destroyIndexBuffer(IndexBuffer* buffer);