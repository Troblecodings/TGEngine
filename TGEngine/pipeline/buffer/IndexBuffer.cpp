#include "IndexBuffer.hpp"

uint32_t index_offset = 0;

void createIndexBuffer(IndexBuffer* buffer) {
	vlib_buffer_create_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	vlib_buffer_create_info.size = sizeof(uint32_t) * buffer->size;
	lastResult = vkCreateBuffer(device, &vlib_buffer_create_info, nullptr, &buffer->index_buffer);
	HANDEL(lastResult)

		vkGetBufferMemoryRequirements(device, buffer->index_buffer, &lastRequirements);

	buffer->max_size = (uint32_t)(vlib_buffer_memory_allocate_info.allocationSize = lastRequirements.size);
	vlib_buffer_memory_allocate_info.memoryTypeIndex = vlib_device_local_memory_index;
	lastResult = vkAllocateMemory(device, &vlib_buffer_memory_allocate_info, nullptr, &buffer->device_memory);
	HANDEL(lastResult)

		lastResult = vkBindBufferMemory(device, buffer->index_buffer, buffer->device_memory, 0);
	HANDEL(lastResult)

		buffer->stag_buf.destination = &buffer->index_buffer;
	buffer->stag_buf.size = vlib_buffer_create_info.size;
	createStagingBuffer(&buffer->stag_buf);
}

void destroyIndexBuffer(IndexBuffer* buffer) {
	vkDestroyBuffer(device, buffer->index_buffer, nullptr);
	vkFreeMemory(device, buffer->device_memory, nullptr);
}

void IndexBuffer::start() {
	this->index_count = 0;
	vkMapMemory(device, this->stag_buf.staging_buffer_device_memory, 0, VK_WHOLE_SIZE, 0, &this->memory);
}

void IndexBuffer::addIndex(uint32_t index) {
	memcpy((uint32_t*)this->memory + this->index_count, &index, sizeof(uint32_t));
	this->index_count++;
}

void IndexBuffer::end() {
	vkUnmapMemory(device, this->stag_buf.staging_buffer_device_memory);
}