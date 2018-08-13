#include "IndexBuffer.hpp"

uint32_t index_count;
uint32_t index_buffer_index;

void createIndexBuffer(uint32_t size) {
	VkBufferCreateInfo index_buffer_create_info = {
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		nullptr,
		0,
		sizeof(uint32_t) * size,
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		0,
		nullptr
	};
	last_result = vkCreateBuffer(device, &index_buffer_create_info, nullptr, &index_buffer);
	HANDEL(last_result)

	index_buffer_index = getMemoryRequirements(index_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void fillIndexBuffer(std::vector<uint32_t>* indicies) {
	index_count = indicies->size();
	void* memory;
	mapMemory(index_buffer_index, &memory);

	memcpy(memory, indicies->data(), sizeof(uint32_t) * index_count);

	unmapMemory();
}