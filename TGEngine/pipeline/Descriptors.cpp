#include "Descriptors.hpp"

VkDescriptorPool descriptor_pool;
std::vector<VkDescriptorSet> descriptor_set;
std::vector<VkDescriptorSetLayout> descriptor_set_layouts;
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
	TG_VECTOR_APPEND_NORMAL(descriptor_set_layouts, VK_NULL_HANDLE)
	vlib_descriptor_set_layout_create_info.bindingCount = (uint32_t)descriptor_bindings.size();
	vlib_descriptor_set_layout_create_info.pBindings = descriptor_bindings.data();
	last_result = vkCreateDescriptorSetLayout(device, &vlib_descriptor_set_layout_create_info, nullptr, &descriptor_set_layouts[last_result]);
	HANDEL(last_result)
}

void initDescriptors() {
	VkDescriptorPoolSize sizes[2];
	vlib_descriptor_pool_create_info.poolSizeCount = 2;

	if(uniform_count > 0)
	{
		vlib_descriptor_pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		vlib_descriptor_pool_size.descriptorCount = uniform_count;
		sizes[0] = vlib_descriptor_pool_size;
	}

	if (image_sampler > 0)
	{
		vlib_descriptor_pool_size.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		vlib_descriptor_pool_size.descriptorCount = image_sampler;
		sizes[1] = vlib_descriptor_pool_size;
	}

	vlib_descriptor_pool_create_info.pPoolSizes = sizes;
	last_result = vkCreateDescriptorPool(device, &vlib_descriptor_pool_create_info, nullptr, &descriptor_pool);
	HANDEL(last_result)

	vlib_allocate_info.descriptorPool = descriptor_pool;
}

void createDescriptorSet() {
	TG_VECTOR_APPEND_NORMAL(descriptor_set, VK_NULL_HANDLE)

	vlib_allocate_info.descriptorSetCount = 1;
	vlib_allocate_info.pSetLayouts = &descriptor_set_layouts[last_result];
	last_result = vkAllocateDescriptorSets(device, &vlib_allocate_info, &descriptor_set[last_result]);
	HANDEL(last_result)
}

void updateDescriptorSet(Descriptor* desc, uint32_t size) {
	VkWriteDescriptorSet descriptor_writes = {
		VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		nullptr,
		nullptr,
		0,
		0,
		1,
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		nullptr,
		nullptr,
		nullptr
	};

	descriptor_writes.descriptorType = desc->type;
	descriptor_writes.dstArrayElement = desc->array_index;
	descriptor_writes.dstBinding = desc->binding;
	descriptor_writes.descriptorCount = desc->count;
	descriptor_writes.dstSet = descriptor_set[desc->descriptor_set];

	if (desc->type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
		desc->buffer_info = {
			buffers[desc->buffer],
			0ULL,
			size,
		};

		descriptor_writes.pImageInfo = nullptr;
		descriptor_writes.pBufferInfo = &desc->buffer_info;
	}
	else if (desc->type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
		desc->desc_image_info = {
			desc->image_sampler,
			desc->image_view,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		};
		
		descriptor_writes.pBufferInfo = nullptr;
		descriptor_writes.pImageInfo = &desc->desc_image_info;
	}

	vkUpdateDescriptorSets(device, 1, &descriptor_writes, 0, nullptr);

}

void destroyDescriptors() {
	for each (VkDescriptorSetLayout var in descriptor_set_layouts)
	{
		vkDestroyDescriptorSetLayout(device, var, nullptr);
	}
	for each (VkDescriptorSet var in descriptor_set)
	{
		last_result = vkFreeDescriptorSets(device, descriptor_pool, 1, &var);
		HANDEL(last_result);
	}
	vkDestroyDescriptorPool(device, descriptor_pool, nullptr);
};