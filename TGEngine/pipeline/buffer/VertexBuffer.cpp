#include "VertexBuffer.hpp"

size_t vertex_offset = 0;

void createVertexBuffer(VertexBuffer* buffer_storage) {
	vlib_buffer_create_info.size = VERTEX_SIZE * buffer_storage->max_vertex_count;
	vlib_buffer_create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	lastResult = vkCreateBuffer(device, &vlib_buffer_create_info, nullptr, &buffer_storage->vertex_buffer);
	HANDEL(lastResult)

		vkGetBufferMemoryRequirements(device, buffer_storage->vertex_buffer, &lastRequirements);

	buffer_storage->max_size = vlib_buffer_memory_allocate_info.allocationSize = lastRequirements.size;
	vlib_buffer_memory_allocate_info.memoryTypeIndex = vlib_device_local_memory_index;
	lastResult = vkAllocateMemory(device, &vlib_buffer_memory_allocate_info, nullptr, &buffer_storage->vertex_buffer_memory);
	HANDEL(lastResult)

		lastResult = vkBindBufferMemory(device, buffer_storage->vertex_buffer, buffer_storage->vertex_buffer_memory, 0);
	HANDEL(lastResult)

		buffer_storage->stag_buf.destination = &buffer_storage->vertex_buffer;
	buffer_storage->stag_buf.size = vlib_buffer_create_info.size;
	createStagingBuffer(&buffer_storage->stag_buf);
}

void destroyVertexBuffer(VertexBuffer* buffer_storage) {
	vkFreeMemory(device, buffer_storage->vertex_buffer_memory, nullptr);
	vkDestroyBuffer(device, buffer_storage->vertex_buffer, nullptr);
}

void VertexBuffer::start() {
	vkMapMemory(device, this->stag_buf.staging_buffer_device_memory, 0, VK_WHOLE_SIZE, 0, &this->memory);
}

VertexBuffer* VertexBuffer::add(glm::vec2 vec) {
	memcpy((uint8_t*)this->memory + this->offset, (uint8_t*)& vec, sizeof(vec));
	this->offset += sizeof(vec);
	return this;
}

VertexBuffer* VertexBuffer::add(glm::vec3 vec) {
	memcpy((uint8_t*)this->memory + this->offset, (uint8_t*)& vec, sizeof(vec));
	this->offset += sizeof(vec);
	return this;
}

VertexBuffer* VertexBuffer::add(glm::vec4 vec) {
	memcpy((uint8_t*)this->memory + this->offset, (uint8_t*)& vec, sizeof(vec));
	this->offset += sizeof(vec);
	return this;
}

VertexBuffer* VertexBuffer::addAll(void* data, size_t size, size_t points) {
	memcpy((uint8_t*)this->memory + this->offset, data, size);
	this->offset += size;
	this->pointCount += points;
	return this;
}

VertexBuffer* VertexBuffer::endVertex() {
	this->pointCount++;
	return this;
}

void VertexBuffer::end() {
	vkUnmapMemory(device, this->stag_buf.staging_buffer_device_memory);
}
