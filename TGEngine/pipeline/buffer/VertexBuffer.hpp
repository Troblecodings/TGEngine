#pragma once

#include "UniformBuffer.hpp"
#include "StagingBuffer.hpp"

struct VertexBuffer {
	uint32_t maximumVertexCount;
	VkBuffer vertex_buffer;
	VkDeviceMemory vertex_buffer_memory;
	StagingBuffer stag_buf;
	VkDeviceSize max_size;
	size_t pointCount;
	size_t offset;
	void* memory;

	SINCE(0, 0, 2)
		void start();

	SINCE(0, 0, 5)
		VertexBuffer* add(glm::vec2 vec);

	SINCE(0, 0, 5)
		VertexBuffer* add(glm::vec3 vec);

	SINCE(0, 0, 5)
		VertexBuffer* add(glm::vec4 vec);

	SINCE(0, 0, 5)
		VertexBuffer* addAll(void* data, size_t size, size_t points);

	SINCE(0, 0, 5)
		VertexBuffer* endVertex();

	SINCE(0, 0, 2)
		void end();
};

extern size_t vertex_offset;

SINCE(0, 0, 1)
void createVertexBuffer(VertexBuffer* buffer_storage);

SINCE(0, 0, 4)
void destroyVertexBuffer(VertexBuffer* buffer_storage);