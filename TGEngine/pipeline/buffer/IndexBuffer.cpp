#include "IndexBuffer.hpp"

void createIndexBuffer(IndexBuffer* buffer) {
	vlib_buffer_create_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	vlib_buffer_create_info.size = sizeof(uint32_t) * buffer->size;
	last_result = vkCreateBuffer(device, &vlib_buffer_create_info, nullptr, &buffer->index_buffer);
	HANDEL(last_result)

	buffer->index_buffer_index = addBuffer(buffer->index_buffer);
}

void IndexBuffer::start() {
	void* memory;
	mapMemory(this->index_buffer_index, &this->memory);
}

void IndexBuffer::addIndex(uint32_t index) {
	this->index_count++;
	memcpy(memory, &index, sizeof(uint32_t));
}

void IndexBuffer::end() {
	unmapMemory();
}