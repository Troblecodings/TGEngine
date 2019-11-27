#include "IndexBuffer.hpp"
#include "Memory.hpp"

uint32_t index_offset = 0;

void createIndexBuffer(IndexBuffer* buffer) {
	vlibBufferCreateInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	vlibBufferCreateInfo.size = sizeof(uint32_t) * buffer->maximumIndexCount;
	lastResult = vkCreateBuffer(device, &vlibBufferCreateInfo, nullptr, &buffer->index_buffer);
	HANDEL(lastResult)

		VkMemoryRequirements lastRequirements;
		vkGetBufferMemoryRequirements(device, buffer->index_buffer, &lastRequirements);

	buffer->max_size = (uint32_t)(vlibBufferMemoryAllocateInfo.allocationSize = lastRequirements.size);
	vlibBufferMemoryAllocateInfo.memoryTypeIndex = vlibDeviceLocalMemoryIndex;
	lastResult = vkAllocateMemory(device, &vlibBufferMemoryAllocateInfo, nullptr, &buffer->device_memory);
	HANDEL(lastResult)

		lastResult = vkBindBufferMemory(device, buffer->index_buffer, buffer->device_memory, 0);
	HANDEL(lastResult)

		buffer->stag_buf.destination = &buffer->index_buffer;
	buffer->stag_buf.size = vlibBufferCreateInfo.size;
	createStagingBuffer(&buffer->stag_buf);
}

void destroyIndexBuffer(IndexBuffer* buffer) {
	vkDestroyBuffer(device, buffer->index_buffer, nullptr);
	vkFreeMemory(device, buffer->device_memory, nullptr);
}

void IndexBuffer::start() {
	this->indexCount = 0;
	vkMapMemory(device, this->stag_buf.staging_buffer_device_memory, 0, VK_WHOLE_SIZE, 0, &this->memory);
}

void IndexBuffer::addIndex(uint32_t index) {
	memcpy((uint32_t*)this->memory + this->indexCount, &index, sizeof(uint32_t));
	this->indexCount++;
}

void IndexBuffer::addAll(void* data, size_t points) {
	memcpy((uint32_t*)this->memory + this->indexCount, data, points * sizeof(uint32_t));
	this->indexCount += points;
}


void IndexBuffer::end() {
	vkUnmapMemory(device, this->stag_buf.staging_buffer_device_memory);
}