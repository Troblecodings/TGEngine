#include "VertexBuffer.hpp"

void createVertexBuffer(VertexBuffer* buffer_storage) {
	VkBuffer vertex_buffer;

	vlib_buffer_create_info.size = VERTEX_SIZE * buffer_storage->max_vertex_count;
	vlib_buffer_create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	last_result = vkCreateBuffer(device, &vlib_buffer_create_info, nullptr, &vertex_buffer);
	HANDEL(last_result)

	buffer_storage->vertex_buffer_index = addBuffer(vertex_buffer);
}

void VertexBuffer::start() {
	mapMemory(this->vertex_buffer_index, &this->memory);
	this->count_of_points = 0;
}

void VertexBuffer::add(TGVertex vert) {
	memcpy((TGVertex*)this->memory + this->count_of_points, &vert, VERTEX_SIZE);
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

void VertexBuffer::addAll(TGVertex* verts, uint32_t count) {
	memcpy((TGVertex*)this->memory + this->count_of_points, verts, VERTEX_SIZE * count);
	this->count_of_points += count;
}

void VertexBuffer::end() {
	unmapMemory();
}
