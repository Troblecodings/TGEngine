#include "IndexBuffer.hpp"

uint32_t index_count;
uint32_t index_buffer_index;

void createIndexBuffer(uint32_t size) {
	vlib_buffer_create_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	vlib_buffer_create_info.size = sizeof(uint32_t) * size;
	last_result = vkCreateBuffer(device, &vlib_buffer_create_info, nullptr, &index_buffer);
	HANDEL(last_result)

	index_buffer_index = addBuffer(index_buffer);
}

void fillIndexBuffer(std::vector<uint32_t>* indicies) {
	index_count = indicies->size();
	void* memory;
	mapMemory(index_buffer_index, &memory);

	memcpy(memory, indicies->data(), sizeof(uint32_t) * index_count);

	unmapMemory();
}