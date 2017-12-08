#include "VertexBuffer.hpp"

VkBuffer vertex_buffer;
uint32_t vertex_count;
uint32_t vertex_buffer_index;

void createVertexBuffer(uint32_t max_vertex_count) {
	VkBufferCreateInfo vertex_buffer_create_info = {
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
	    nullptr,
		0,
	    sizeof(Vertex) * max_vertex_count,
	    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
	    VK_SHARING_MODE_EXCLUSIVE,
	    0,
	    nullptr
	};

	last_result = vkCreateBuffer(device, &vertex_buffer_create_info, nullptr, &vertex_buffer);
	HANDEL(last_result)

	vertex_buffer_index = getMemoryRequirements(vertex_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void fillVertexBuffer(std::vector<Vertex>* vertecies) {
	vertex_count = vertecies->size();

	void* memory;
	mapMemory(vertex_buffer_index, &memory);

	memcpy(memory, vertecies->data(), sizeof(Vertex) * vertex_count);

	unmapMemory();
}

void destroyVertexBuffer() {
	vkDestroyBuffer(device, vertex_buffer, nullptr);
}