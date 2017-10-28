#include "VertexBuffer.hpp"

VkBuffer vertex_buffer;

void createVertexBuffer(std::vector<Vertex> vertecies) {
	VkBufferCreateInfo vertex_buffer_create_info = {
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
	    nullptr,
		0,
	    sizeof(Vertex) * vertecies.size(),
	    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
	    VK_SHARING_MODE_EXCLUSIVE,
	    0,
	    nullptr,
	};

	last_result = vkCreateBuffer(device, &vertex_buffer_create_info, nullptr, &vertex_buffer);
	HANDEL(last_result)

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(device, vertex_buffer, &requirements);

	uint32_t index = 0;
	for (index = 0; index < memory_properties.memoryTypeCount; index++) {

		if ((requirements.memoryTypeBits & (1 << index)) && (memory_properties.memoryTypes[index].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) break;
	}

	VkDeviceMemory device_memory;

	VkMemoryAllocateInfo memory_allocation_info = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
	    nullptr,
	    requirements.size,
		index
	};
	last_result = vkAllocateMemory(device, &memory_allocation_info, nullptr, &device_memory);

	last_result = vkBindBufferMemory(device, vertex_buffer, device_memory, requirements.alignment);
	HANDEL(last_result)

	void* memory;
	last_result = vkMapMemory(device, device_memory, requirements.alignment, requirements.size, 0, &memory);
	HANDEL(last_result)

	memcpy(memory, vertecies.data(), vertex_buffer_create_info.size);

	vkUnmapMemory(device, device_memory);
}