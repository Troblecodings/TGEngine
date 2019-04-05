#include "Descriptors.hpp"

VkDescriptorPool descriptor_pool;
std::vector<VkDescriptorSet> descriptor_set;
std::vector<VkDescriptorSetLayout> descriptor_set_layouts;
std::vector<VkDescriptorSetLayoutBinding> descriptor_bindings;

uint32_t uniform_count;
uint32_t image_sampler_count;

void addDescriptorBinding(uint32_t binding, VkDescriptorType type, VkShaderStageFlags flags) {
	descriptor_bindings.push_back( { binding, type, 1, flags });
}

void initDescriptors() {
	vlib_descriptor_pool_create_info.poolSizeCount = (uniform_count > 0 ? 1:0) + (image_sampler_count > 0 ? 1:0);
	VkDescriptorPoolSize* sizes = new VkDescriptorPoolSize[vlib_descriptor_pool_create_info.poolSizeCount];

	if(uniform_count > 0)
	{
		vlib_descriptor_pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		vlib_descriptor_pool_size.descriptorCount = uniform_count;
		sizes[0] = vlib_descriptor_pool_size;
	}

	if (image_sampler_count > 0)
	{
		vlib_descriptor_pool_size.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		vlib_descriptor_pool_size.descriptorCount = image_sampler_count;
		sizes[vlib_descriptor_pool_create_info.poolSizeCount - 1] = vlib_descriptor_pool_size;
	}

	vlib_descriptor_pool_create_info.pPoolSizes = sizes;
	last_result = vkCreateDescriptorPool(device, &vlib_descriptor_pool_create_info, nullptr, &descriptor_pool);
	HANDEL(last_result)

	vlib_allocate_info.descriptorPool = descriptor_pool;
}

size_t createDesctiptorLayout() {
	TG_VECTOR_GET_SIZE_AND_RESIZE(descriptor_set_layouts)

	vlib_descriptor_set_layout_create_info.bindingCount = (uint32_t)descriptor_bindings.size();
	vlib_descriptor_set_layout_create_info.pBindings = descriptor_bindings.data();
	last_result = vkCreateDescriptorSetLayout(device, &vlib_descriptor_set_layout_create_info, nullptr, &descriptor_set_layouts[last_size]);
	HANDEL(last_result)
    return last_size;
}

size_t createDescriptorSet(uint32_t layout) {
	TG_VECTOR_GET_SIZE_AND_RESIZE(descriptor_set)
	vlib_allocate_info.pSetLayouts = &descriptor_set_layouts[layout];
	last_result = vkAllocateDescriptorSets(device, &vlib_allocate_info, &descriptor_set[last_size]);
	HANDEL(last_result)
	return last_size;
}

void updateDescriptorSet(Descriptor* desc, uint32_t size) {
	vlib_descriptor_writes.descriptorType = desc->type;
	vlib_descriptor_writes.dstArrayElement = desc->array_index;
	vlib_descriptor_writes.dstBinding = desc->binding;
	vlib_descriptor_writes.descriptorCount = desc->count;
	vlib_descriptor_writes.dstSet = descriptor_set[desc->descriptor_set];

	if (desc->type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
		desc->buffer_info = {
			buffers[desc->buffer],
			0ULL,
			size,
		};

		vlib_descriptor_writes.pImageInfo = nullptr;
		vlib_descriptor_writes.pBufferInfo = &desc->buffer_info;
	}
	else if (desc->type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
		desc->desc_image_info = new VkDescriptorImageInfo[desc->count];
		for (size_t i = 0; i < desc->count; i++)
		{
			desc->desc_image_info[i] = {
				desc->image_sampler,
				desc->image_view[i],
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			};
		}

		vlib_descriptor_writes.pBufferInfo = nullptr;
		vlib_descriptor_writes.pImageInfo = desc->desc_image_info;
	}

	vkUpdateDescriptorSets(device, 1, &vlib_descriptor_writes, 0, nullptr);

}

void destroyDescriptors() {
	for each (VkDescriptorSetLayout var in descriptor_set_layouts)
	{
		vkDestroyDescriptorSetLayout(device, var, nullptr);
	}
	last_result = vkFreeDescriptorSets(device, descriptor_pool, (uint32_t)descriptor_set.size(), descriptor_set.data());
	HANDEL(last_result);
	vkDestroyDescriptorPool(device, descriptor_pool, nullptr);
};

Descriptor::Descriptor(VkShaderStageFlags stage, VkDescriptorType type)
{
	this->shader_stage = stage;
	this->type = type;
	if (this->type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
		uniform_count++;
	}
	else if (this->type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
		image_sampler_count++;
	}
}
