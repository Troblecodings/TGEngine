#pragma once

#include "Memory.hpp"

struct IndexBuffer{
	uint32_t size;

	VkBuffer index_buffer;
	VkDeviceMemory device_memory;
	uint32_t max_size;
	uint32_t index_count;
	void* memory;

	SINCE(0, 0, 2)
	void addIndex(uint32_t index);
	
	SINCE(0, 0, 4)
	void start();

	SINCE(0, 0, 4)
	void end();
};

SINCE(0, 0, 2)
void createIndexBuffer(IndexBuffer* buffer);

void destroyIndexBuffer(IndexBuffer* buffer);