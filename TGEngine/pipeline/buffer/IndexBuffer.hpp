#pragma once

#include "StagingBuffer.hpp"

struct IndexBuffer {
	uint32_t maximumIndexCount;

	VkBuffer index_buffer;
	VkDeviceMemory device_memory;
	uint32_t max_size;
	uint32_t indexCount;
	StagingBuffer stag_buf;
	void* memory;

	void addIndex(uint32_t index);

	void start();

	void addAll(void* data, size_t points);

	void end();
};

extern uint32_t index_offset;

void createIndexBuffer(IndexBuffer* buffer);

void destroyIndexBuffer(IndexBuffer* buffer);