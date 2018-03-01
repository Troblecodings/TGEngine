#include "Descriptors.hpp"

VkDescriptorPool descriptor_pool;
std::vector<VkDescriptorSet> descriptor_sets;
std::vector<uint32_t> buffers_for_descriptor = {};

void addUniformBuffer(Descriptor* buffer) {
	buffer->descriptor = buffers_for_descriptor.size();
	buffers_for_descriptor.resize(buffer->descriptor + 1);
	buffers_for_descriptor[buffer->descriptor] = buffer->buffer;
}

void createAllDescriptorSets() {
	VkDescriptorPoolSize descriptor_pool_size = {
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		buffers_for_descriptor.size()
	};

	VkDescriptorPoolCreateInfo descriptor_pool_create_info = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		nullptr,
		VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
		1,
		1,
		&descriptor_pool_size
	};

	last_result = vkCreateDescriptorPool(device, &descriptor_pool_create_info, nullptr, &descriptor_pool);
	HANDEL(last_result)

	descriptor_sets.resize(buffers_for_descriptor.size());
	VkDescriptorSetAllocateInfo allocate_info = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		nullptr,
		descriptor_pool,
		descriptor_sets.size(),
		&descriptor_set_layout
	};
	last_result = vkAllocateDescriptorSets(device, &allocate_info, descriptor_sets.data());
	HANDEL(last_result)
}

void destroyDescriptors() {
	vkFreeDescriptorSets(device, descriptor_pool, descriptor_sets.size(), descriptor_sets.data());
	vkDestroyDescriptorPool(device, descriptor_pool, nullptr);
};

void updateDescriptorSet(Descriptor* desc, uint32_t size) {

	VkDescriptorBufferInfo buffer_info = {
		buffers[desc->buffer],
		0,
		size
	};

	VkWriteDescriptorSet descriptor_writes = {
		VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		nullptr,
		descriptor_sets[desc->descriptor],
		desc->binding,
		0,
		1,
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		nullptr,
		&buffer_info,
		nullptr
	};

	vkUpdateDescriptorSets(device, 1, &descriptor_writes, 0, nullptr);
}

void addDescriptor(Descriptor* descriptor) {
	uint32_t csize = descriptor_set_layout_bindings.size();
	descriptor_set_layout_bindings.resize(csize + 1);
	descriptor_set_layout_bindings[csize] = {
		descriptor->binding,
		descriptor->type,
		descriptor->count,
		descriptor->shader_stage,
		nullptr
	};
}