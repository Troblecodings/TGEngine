#pragma once

#include "../../pipeline/Device.hpp"
#include "../../util/TGVertex.hpp"
#include "Memory.hpp"
#include "UniformBuffer.hpp"
#include "StagingBuffer.hpp"

struct VertexBuffer {
	INPUT uint32_t max_vertex_count;
	OUTPUT 
	VkBuffer vertex_buffer;
	VkDeviceMemory vertex_buffer_memory;
	StagingBuffer stag_buf;
	VkDeviceSize max_size;
	size_t count_of_points;
	void* memory;

	SINCE(0, 0, 2)
	void start();

	SINCE(0, 0, 2)
	void add(TGVertex vert);

	SINCE(0, 0, 3)
	void addColorOnly(TGVertex vert);

	SINCE(0, 0, 3)
	void addTexOnly(TGVertex vert);

	SINCE(0, 0, 2)
	void addAll(TGVertex* verts, size_t count);

	SINCE(0, 0, 2)
	void end();
};

extern size_t vertex_offset;

SINCE(0, 0, 1)
void createVertexBuffer(VertexBuffer* buffer_storage);

SINCE(0, 0, 4)
void destroyVertexBuffer(VertexBuffer* buffer_storage);