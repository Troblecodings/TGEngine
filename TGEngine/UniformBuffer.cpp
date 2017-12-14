#include "UniformBuffer.hpp"

uint32_t createUniformBuffer(uint32_t size) {

	VkBuffer uniform_buffer;

	VkBufferCreateInfo uniform_buffer_create_info = {
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		nullptr,
		0,
		size,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		0,
		nullptr
	};

	last_result = vkCreateBuffer(device, &uniform_buffer_create_info, nullptr, &uniform_buffer);
	HANDEL(last_result)

	return getMemoryRequirements(uniform_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void fillUniformBuffer(uint32_t index, void* input,uint32_t size) {
	void* memory;
	mapMemory(index, &memory);
	memcpy(memory, input, size);
	unmapMemory();
}