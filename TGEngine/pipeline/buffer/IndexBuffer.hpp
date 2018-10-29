#pragma once

#include "Memory.hpp"

struct IndexBuffer{
	uint32_t size;

	VkBuffer index_buffer;
	uint32_t index_count;
	uint32_t index_buffer_index;
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