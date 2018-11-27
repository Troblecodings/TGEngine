#include "IndexBuffer.hpp"

void createIndexBuffer(IndexBuffer* buffer) {
	vlib_buffer_create_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	vlib_buffer_create_info.size = sizeof(uint32_t) * buffer->size;
	last_result = vkCreateBuffer(device, &vlib_buffer_create_info, nullptr, &buffer->index_buffer);
	HANDEL(last_result)

	vkGetBufferMemoryRequirements(device, buffer->index_buffer, &last_requirements);

	buffer->max_size = vlib_buffer_memory_allocate_info.allocationSize = last_requirements.size;
	vlib_buffer_memory_allocate_info.memoryTypeIndex = vlib_device_host_visible_coherent_index;
	last_result = vkAllocateMemory(device, &vlib_buffer_memory_allocate_info, nullptr, &buffer->device_memory);
	HANDEL(last_result)

	last_result = vkBindBufferMemory(device, buffer->index_buffer, buffer->device_memory, 0);
	HANDEL(last_result)
}

void destroyIndexBuffer(IndexBuffer * buffer)
{
	vkDestroyBuffer(device, buffer->index_buffer, nullptr);
	vkFreeMemory(device, buffer->device_memory, nullptr);
}

void IndexBuffer::start() {
	this->index_count = 0;
	vkMapMemory(device, this->device_memory, 0, this->max_size, 0, &this->memory);
}

void IndexBuffer::addIndex(uint32_t index) {
	memcpy((uint32_t*)this->memory + this->index_count, &index, sizeof(uint32_t));
	this->index_count++;
}

void IndexBuffer::end() {
	vkUnmapMemory(device, this->device_memory);
}