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

	void start();

	VertexBuffer* add(glm::vec2 vec);

	VertexBuffer* add(glm::vec3 vec);

	VertexBuffer* add(glm::vec4 vec);

	VertexBuffer* addAll(void* data, size_t size, size_t points);

	VertexBuffer* endVertex();

	void end();
};

extern size_t vertex_offset;

void createVertexBuffer(VertexBuffer* buffer_storage);

void destroyVertexBuffer(VertexBuffer* buffer_storage);