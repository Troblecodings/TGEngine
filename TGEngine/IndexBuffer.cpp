#include "IndexBuffer.hpp"

VkBuffer index_buffer;
VkMemoryRequirements index_buffer_requirements;
VkDeviceMemory index_memory;
uint32_t index_count;

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

	vkGetBufferMemoryRequirements(device, index_buffer, &index_buffer_requirements);

	uint32_t index = 0;
	for (index = 0; index < memory_properties.memoryTypeCount; index++) {
		if (memory_properties.memoryTypes[index].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) break;
	}

	VkMemoryAllocateInfo memory_allocation_info = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		nullptr,
		index_buffer_requirements.size,
		index
	};
	last_result = vkAllocateMemory(device, &memory_allocation_info, nullptr, &index_memory);

	last_result = vkBindBufferMemory(device, index_buffer, index_memory, 0);
	HANDEL(last_result)
}

void fillIndexBuffer(std::vector<uint32_t>* indicies) {
	index_count = indicies->size();
	void* memory;
	last_result = vkMapMemory(device, index_memory, 0, index_buffer_requirements.size, 0, &memory);
	HANDEL(last_result)

	memccpy(memory, indicies->data(), 0, sizeof(uint32_t) * index_count);

	vkUnmapMemory(device, index_memory);
}

void destroyIndexBuffer() {
	vkFreeMemory(device, index_memory, nullptr);
	vkDestroyBuffer(device, index_buffer, nullptr);
}