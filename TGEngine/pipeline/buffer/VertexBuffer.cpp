#include "VertexBuffer.hpp"

size_t vertex_offset = 0;

void createVertexBuffer(VertexBuffer* buffer_storage) {
	vlib_buffer_create_info.size = VERTEX_SIZE * buffer_storage->max_vertex_count;
	vlib_buffer_create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	last_result = vkCreateBuffer(device, &vlib_buffer_create_info, nullptr, &buffer_storage->vertex_buffer);
	HANDEL(last_result)
		
	vkGetBufferMemoryRequirements(device, buffer_storage->vertex_buffer, &last_requirements);

	buffer_storage->max_size = vlib_buffer_memory_allocate_info.allocationSize = last_requirements.size;
	vlib_buffer_memory_allocate_info.memoryTypeIndex = vlib_device_local_memory_index;
	last_result = vkAllocateMemory(device, &vlib_buffer_memory_allocate_info, nullptr, &buffer_storage->vertex_buffer_memory);
	HANDEL(last_result)

	last_result = vkBindBufferMemory(device, buffer_storage->vertex_buffer, buffer_storage->vertex_buffer_memory, 0);
	HANDEL(last_result)

	buffer_storage->stag_buf.destination = &buffer_storage->vertex_buffer;
	buffer_storage->stag_buf.size = vlib_buffer_create_info.size;
	createStagingBuffer(&buffer_storage->stag_buf);
}

void destroyVertexBuffer(VertexBuffer * buffer_storage)
{
	vkFreeMemory(device, buffer_storage->vertex_buffer_memory, nullptr);
	vkDestroyBuffer(device, buffer_storage->vertex_buffer, nullptr);
}

void VertexBuffer::start() {
	vkMapMemory(device, this->stag_buf.staging_buffer_device_memory, vertex_offset * VERTEX_SIZE, VK_WHOLE_SIZE, 0, &this->memory);
}

void VertexBuffer::add(TGVertex vert) {
	memcpy((TGVertex*)this->memory + this->count_of_points - vertex_offset, &vert, VERTEX_SIZE);
	this->count_of_points++;
}

void VertexBuffer::addColorOnly(TGVertex vert) {
	vert.color_only = VK_TRUE;
	this->add(vert);
}

void VertexBuffer::addTexOnly(TGVertex vert) {
	vert.color_only = VK_FALSE;
	vert.color = { 1, 1, 1, 1 } ;
	this->add(vert);
}

void VertexBuffer::addAll(TGVertex* verts, size_t count) {
	memcpy((TGVertex*)this->memory + this->count_of_points - vertex_offset, verts, VERTEX_SIZE * count);
	this->count_of_points += count;
}

void VertexBuffer::end() {
	vkUnmapMemory(device, this->stag_buf.staging_buffer_device_memory);
}
