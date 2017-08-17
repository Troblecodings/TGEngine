#include "VertexBuffer.h"

namespace Pipeline {

	using namespace std;

	void createVertexBuffer(VertexBuffer* buffer) {
		VkBufferCreateInfo buffer_create_info = {};
		buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffer_create_info.pNext = nullptr;
		buffer_create_info.flags = 0;
		buffer_create_info.size = buffer->vertecies->size() * sizeof((*buffer->vertecies)[0]);
		buffer_create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		vector<uint32_t> queuefamalieIndeces = {buffer->device->queuFamalieindex};
		buffer_create_info.queueFamilyIndexCount = queuefamalieIndeces.size();
		buffer_create_info.pQueueFamilyIndices = queuefamalieIndeces.data();
        
		buffer->buffer = new VkBuffer;
		handel(vkCreateBuffer(*buffer->device->device, &buffer_create_info, nullptr, buffer->buffer));

		buffer->requirements = new VkMemoryRequirements;
		vkGetBufferMemoryRequirements(*buffer->device->device, *buffer->buffer, buffer->requirements);

		uint32_t index = 0;
		uint32_t properties = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		for (index = 0; index < buffer->device->memoryprops->memoryTypeCount; index++) {

			if ((buffer->requirements->memoryTypeBits & (1 << index)) && (buffer->device->memoryprops->memoryTypes[index].propertyFlags & properties) == properties) break;
		}

		VkMemoryAllocateInfo allocate_info = {};
		allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocate_info.pNext = nullptr;
		allocate_info.allocationSize = buffer->requirements->size;
		allocate_info.memoryTypeIndex = index;

		buffer->memory = new VkDeviceMemory;
		handel(vkAllocateMemory(*buffer->device->device, &allocate_info, nullptr, buffer->memory));

		handel(vkBindBufferMemory(*buffer->device->device, *buffer->buffer, *buffer->memory, 0));

		size_t size = buffer_create_info.size;
		void* data;
		handel(vkMapMemory(*buffer->device->device, *buffer->memory, 0, size, 0, &data));
		memcpy(data, buffer->vertecies->data(), size);
		vkUnmapMemory(*buffer->device->device, *buffer->memory);
	}

	void fillBuffer(VertexBuffer* buffer){
		
	}

	void destroyVertexBuffer(VertexBuffer* buffer) {
		vkDestroyBuffer(*buffer->device->device, *buffer->buffer, nullptr);
		vkFreeMemory(*buffer->device->device, *buffer->memory, nullptr);
	}

}