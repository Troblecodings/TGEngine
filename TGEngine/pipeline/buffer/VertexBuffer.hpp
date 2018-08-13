#pragma once

#include "..\..\pipeline\Device.hpp"
#include "..\..\TGVertex.hpp"
#include "Memory.hpp"
#include "UniformBuffer.hpp"

struct VertexBuffer {
	INPUT uint32_t max_vertex_count;
	OUTPUT uint32_t vertex_buffer_index;
	OUTPUT uint32_t count_of_points;
	OUTPUT void* memory;

	SINCE(0, 0, 2)
	void start();

	SINCE(0, 0, 2)
	void add(TGVertex vert);

	SINCE(0, 0, 3)
	void addColorOnly(TGVertex vert);

	SINCE(0, 0, 3)
	void addTexOnly(TGVertex vert);

	SINCE(0, 0, 2)
	void addAll(TGVertex* verts, uint32_t count);

	SINCE(0, 0, 2)
	void end();
};

SINCE(0, 0, 1)
void createVertexBuffer(VertexBuffer* buffer_storage);