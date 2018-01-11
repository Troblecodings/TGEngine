#pragma once

#include "Device.hpp"
#include "Vertex.hpp"
#include "Memory.hpp"

struct VertexBuffer {
	uint32_t max_vertex_count;
	uint32_t vertex_buffer_index;
	uint32_t count_of_points;
	void** memory;

	SINCE(0, 0, 2)
	void start();

	SINCE(0, 0, 2)
	void add(Vertex vert);

	SINCE(0, 0, 2)
	void addAll(Vertex* verts, uint32_t count);

	SINCE(0, 0, 2)
	void end();
};

SINCE(0, 0, 1)
void createVertexBuffer(VertexBuffer* buffer_storage);