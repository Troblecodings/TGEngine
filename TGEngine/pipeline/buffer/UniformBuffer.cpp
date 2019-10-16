#include "UniformBuffer.hpp"

UniformBuffer::UniformBuffer(uint32_t size, VkShaderStageFlags flags, uint32_t binding) : size(size) {
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
	VkDescriptorBufferInfo descriptorBufferInfo;
	descriptorBufferInfo.buffer = buffers[index];
	descriptorBufferInfo.offset = buffer_offsets[index];
	descriptorBufferInfo.range = buffer_sizes[index];

	VkWriteDescriptorSet writeDescriptorSet;
	writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.pNext = nullptr;
	writeDescriptorSet.dstSet = mainDescriptorSet;
	writeDescriptorSet.dstBinding = 0;
	writeDescriptorSet.dstArrayElement = 0;
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writeDescriptorSet.pImageInfo = nullptr;
	writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;
	writeDescriptorSet.pTexelBufferView = nullptr;
	vkUpdateDescriptorSets(device, 1, &writeDescriptorSet, 0, nullptr);
}

void fillUniformBuffer(UniformBuffer* buffer, void* input, uint32_t size, uint32_t offset) {
	vkDeviceWaitIdle(device);
	mapMemory(buffer->index, &buffer->memory, size, offset);
	memcpy(buffer->memory, input, size);
	unmapMemory();
}