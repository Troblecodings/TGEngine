#include "Descriptors.hpp"

VkDescriptorPool descriptor_pool;
VkDescriptorSet descriptor_set;
VkDescriptorSetLayout desc_layout;
std::vector<VkDescriptorSetLayoutBinding> descriptor_bindings;

uint32_t uniform_count;
uint32_t image_sampler;

void addDescriptor(Descriptor* descriptor) {
	descriptor->binding = (uint32_t)descriptor_bindings.size();
	descriptor_bindings.resize(descriptor->binding + 1);
	descriptor_bindings[descriptor->binding] = {
		descriptor->binding,
		descriptor->type,
		descriptor->count,
		descriptor->shader_stage,
		nullptr
	};

	if (descriptor->type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
		uniform_count++;
	}
	else if (descriptor->type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
		image_sampler++;
	}
}

void createPipelineLayout() {
	VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		nullptr,
		0,
		(uint32_t)descriptor_bindings.size(),
		descriptor_bindings.data()
	};
	last_result = vkCreateDescriptorSetLayout(device, &descriptor_set_layout_create_info, nullptr, &desc_layout);
	HANDEL(last_result)
}

void createAllDescriptorSets() {
	std::vector<VkDescriptorPoolSize> sizes;

	size_t c_size;
	if (uniform_count > 0) {
		VkDescriptorPoolSize descriptor_pool_size = {
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			uniform_count
		};
		c_size = sizes.size();
		sizes.resize(c_size + 1);
		sizes[c_size] = descriptor_pool_size;
	}

	if (image_sampler > 0) {
		VkDescriptorPoolSize descriptor_pool_size = {
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			image_sampler
		};
		c_size = sizes.size();
		sizes.resize(c_size + 1);
		sizes[c_size] = descriptor_pool_size;
	}

	VkDescriptorPoolCreateInfo descriptor_pool_create_info = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		nullptr,
		VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
		1,
		(uint32_t)sizes.size(),
		sizes.data()
	};
	last_result = vkCreateDescriptorPool(device, &descriptor_pool_create_info, nullptr, &descriptor_pool);
	HANDEL(last_result)

	VkDescriptorSetAllocateInfo allocate_info = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		nullptr,
		descriptor_pool,
		1,
		&desc_layout
	};
	last_result = vkAllocateDescriptorSets(device, &allocate_info, &descriptor_set);
	HANDEL(last_result)
}

void updateDescriptorSet(Descriptor* desc, uint32_t size) {

	VkWriteDescriptorSet descriptor_writes;

	if (desc->type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
		desc->buffer_info = {
			buffers[desc->buffer],
			0ULL,
			size
		};

		descriptor_writes = {
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			nullptr,
			descriptor_set,
			desc->binding,
			desc->array_index,
			1,
			desc->type,
			nullptr,
			&desc->buffer_info,
			nullptr
		};
	}
	else if (desc->type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
		desc->desc_image_info = {
			desc->image_sampler,
			desc->image_view,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		};
		descriptor_writes = {
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			nullptr,
			descriptor_set,
			desc->binding,
			desc->array_index,
			1,
			desc->type,
			&desc->desc_image_info,
			nullptr,
			nullptr
		};
	}

	vkUpdateDescriptorSets(device, 1, &descriptor_writes, 0, nullptr);
}

void destroyDescriptors() {
	vkDestroyDescriptorSetLayout(device, desc_layout, nullptr);
	last_result = vkFreeDescriptorSets(device, descriptor_pool, 1, &descriptor_set);
	HANDEL(last_result);
	vkDestroyDescriptorPool(device, descriptor_pool, nullptr);
};