#include "UniformBuffer.hpp"

UniformBuffer::UniformBuffer(uint32_t size, VkShaderStageFlags flags, uint32_t binding) : size(size), descriptor(Descriptor(flags, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, binding)) {
	VkBuffer uniform_buffer;

	VkBufferCreateInfo uniform_buffer_create_info = {
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		nullptr,
		0,
		this->size,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		0,
		nullptr
	};

	lastResult = vkCreateBuffer(device, &uniform_buffer_create_info, nullptr, &uniform_buffer);
	HANDEL(lastResult)

		this->index = addBuffer(uniform_buffer);
}

void UniformBuffer::updateDescriptor() {
	this->descriptor.updateBufferInfo(this->index, this->size);
}

void fillUniformBuffer(UniformBuffer* buffer, void* input, uint32_t size) {
	vkDeviceWaitIdle(device);
	mapMemory(buffer->index, &buffer->memory);
	memcpy(buffer->memory, input, size);
	unmapMemory();
}