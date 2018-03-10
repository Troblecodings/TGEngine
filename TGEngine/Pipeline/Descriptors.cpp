#include "Descriptors.hpp"

VkDescriptorPool descriptor_pool;
std::vector<VkDescriptorSet> descriptor_sets;

void createAllDescriptorSets() {

	VkDescriptorPoolSize descriptor_pool_size = {
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		descriptor_sets.size()
	};

	VkDescriptorPoolCreateInfo descriptor_pool_create_info = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		nullptr,
		VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
		descriptor_sets.size(),
		1,
		&descriptor_pool_size
	};
	last_result = vkCreateDescriptorPool(device, &descriptor_pool_create_info, nullptr, &descriptor_pool);
	HANDEL(last_result)

	VkDescriptorSetAllocateInfo allocate_info = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		nullptr,
		descriptor_pool,
		descriptor_set_layout.size(),
		descriptor_set_layout.data()
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
		buffers[desc->binding],
		0,
		size
	};

	VkWriteDescriptorSet descriptor_writes = {
		VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		nullptr,
		descriptor_sets[desc->binding],
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
	uint32_t csize = descriptor_sets.size();
	descriptor_sets.resize(csize + 1);
	descriptor_set_layout_bindings.resize(csize + 1);
	descriptor_set_layout_bindings[csize] = {
		csize,
		descriptor->type,
		descriptor->count,
		descriptor->shader_stage,
		nullptr
	};
	descriptor->binding = csize;
	
	descriptor_set_layout.resize(csize + 1);
	VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		nullptr,
		0,
		1,
		&descriptor_set_layout_bindings[csize]
	};
	last_result = vkCreateDescriptorSetLayout(device, &descriptor_set_layout_create_info, nullptr, &descriptor_set_layout[csize]);
	HANDEL(last_result)
}