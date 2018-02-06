#pragma once

#include "..\..\Pipeline\Device.hpp"
#include "..\..\Util\Vertex.hpp"
#include "Memory.hpp"

struct VertexBuffer {
	INPUT uint32_t max_vertex_count;
	OUTPUT uint32_t vertex_buffer_index;
	OUTPUT uint32_t count_of_points;
	OUTPUT void* memory;

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