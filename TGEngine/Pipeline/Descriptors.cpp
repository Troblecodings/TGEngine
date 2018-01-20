#include "Descriptors.hpp"

VkDescriptorPool descriptor_pool;
std::vector<VkDescriptorSet> descriptor_sets;
std::vector<uint32_t> descriptors;

void createDescriptorsForUniformBuffers(std::vector<uint32_t> descriptor) {
	descriptors = descriptor;

	VkDescriptorPoolSize descriptor_pool_size = {
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		descriptors.size()
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

	descriptor_sets.resize(descriptors.size());
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

void updateDescriptorSet(uint32_t index,uint32_t binding, uint32_t size) {
	uint32_t desc_index = getDescriptorIndex(index);

	OUT_LV_DEBUG(desc_index)

	VkDescriptorBufferInfo buffer_info = {
		buffers[index],
		0,
		size
	};

	VkWriteDescriptorSet descriptor_writes = {
		VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		nullptr,
		descriptor_sets[desc_index],
		binding,
		0,
		1,
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		nullptr,
		&buffer_info,
		nullptr
	};

	vkUpdateDescriptorSets(device, 1, &descriptor_writes, 0, nullptr);
}

uint32_t getDescriptorIndex(uint32_t buffer_index) {
	uint32_t rtn = 0;
	for (uint32_t val : descriptors) {
		if (val == buffer_index) {
			return rtn;
		}
		rtn++;
	}
	return rtn;
}