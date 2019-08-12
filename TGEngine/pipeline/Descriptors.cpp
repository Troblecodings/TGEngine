#include "Descriptors.hpp"
#include "../vlib/VulkanDescriptor.hpp"

VkDescriptorPool descriptor_pool;
std::vector<VkDescriptorSet> descriptorSets;
std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
std::vector<VkDescriptorSetLayoutBinding> descriptor_bindings;
std::vector<VkPipelineLayout> pipeLayouts;

uint32_t uniform_count;
uint32_t image_sampler_count;

void addDescriptorBinding(uint32_t binding, VkDescriptorType type, VkShaderStageFlags flags) {
	descriptor_bindings.push_back({ binding, type, 1, flags });
}

void initDescriptors() {
	vlib_descriptor_pool_create_info.poolSizeCount = 2;
	VkDescriptorPoolSize* sizes = new VkDescriptorPoolSize[vlib_descriptor_pool_create_info.poolSizeCount];


	vlib_descriptor_pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vlib_descriptor_pool_size.descriptorCount = TGE_MIN(device_properties.limits.maxDescriptorSetUniformBuffers, 20);
	sizes[0] = vlib_descriptor_pool_size;

	vlib_descriptor_pool_size.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	vlib_descriptor_pool_size.descriptorCount = TGE_MIN(device_properties.limits.maxDescriptorSetSamplers, 20);
	sizes[1] = vlib_descriptor_pool_size;

	vlib_descriptor_pool_create_info.pPoolSizes = sizes;
	lastResult = vkCreateDescriptorPool(device, &vlib_descriptor_pool_create_info, nullptr, &descriptor_pool);
	HANDEL(lastResult)

	vlib_allocate_info.descriptorPool = descriptor_pool;
}

uint32_t createLayouts() {
	TG_VECTOR_GET_SIZE_AND_RESIZE(pipeLayouts)
	TG_VECTOR_GET_SIZE_AND_RESIZE(descriptorSetLayouts)

	lastResult = vkCreateDescriptorSetLayout(device, &vlib_descriptor_set_layout_create_info, nullptr, &descriptorSetLayouts[lastSize]);
	HANDEL(lastResult)

	vlib_layout_info.pSetLayouts = &descriptorSetLayouts[lastSize];
	lastResult = vkCreatePipelineLayout(device, &vlib_layout_info, nullptr, &pipeLayouts[lastSize]);
	HANDEL(lastResult);

		return (uint32_t)lastSize;
}

void destroyDesctiptorLayout(uint32_t layout) {
	vkDestroyDescriptorSetLayout(device, descriptorSetLayouts[layout], nullptr);
}

void destroyDescriptorSet(uint32_t layout) {
	lastResult = vkFreeDescriptorSets(device, descriptor_pool, 1, &descriptorSets[layout]);
	HANDEL(lastResult);
}

uint32_t createDescriptorSet(uint32_t layout) {
	TG_VECTOR_GET_SIZE_AND_RESIZE(descriptorSets)

		vlib_allocate_info.pSetLayouts = &descriptorSetLayouts[layout];
	lastResult = vkAllocateDescriptorSets(device, &vlib_allocate_info, &descriptorSets[lastSize]);
	HANDEL(lastResult)
		return (uint32_t)lastSize;
}

void destroyDescriptors() {
	for each(VkDescriptorSetLayout var in descriptorSetLayouts) {
		vkDestroyDescriptorSetLayout(device, var, nullptr);
	}
	lastResult = vkFreeDescriptorSets(device, descriptor_pool, (uint32_t)descriptorSets.size(), descriptorSets.data());
	HANDEL(lastResult);
	descriptorSets.clear();
	descriptorSetLayouts.clear();
};

void Descriptor::updateImageInfo(VkSampler sampler, VkImageView view) {
	if(this->type != VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) return;
	update();

	VkDescriptorImageInfo imageinfo;
	imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageinfo.imageView = view;
	imageinfo.sampler = sampler;

	vlib_descriptor_writes.pBufferInfo = nullptr;
	vlib_descriptor_writes.pImageInfo = &imageinfo;

	vkUpdateDescriptorSets(device, 1, &vlib_descriptor_writes, 0, nullptr);
}

void Descriptor::updateBufferInfo(uint32_t buffer, size_t size) {
	if(this->type != VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) return;
	update();

	VkDescriptorBufferInfo bufferinfo;
	bufferinfo.buffer = buffers[buffer];
	bufferinfo.range = size;
	bufferinfo.offset = 0;

	vlib_descriptor_writes.pBufferInfo = &bufferinfo;
	vlib_descriptor_writes.pImageInfo = nullptr;

	vkUpdateDescriptorSets(device, 1, &vlib_descriptor_writes, 0, nullptr);
}

void Descriptor::update() {
	vlib_descriptor_writes.descriptorType = this->type;
	vlib_descriptor_writes.dstArrayElement = 0;
	vlib_descriptor_writes.dstBinding = this->binding;
	vlib_descriptor_writes.descriptorCount = 1;
	vlib_descriptor_writes.dstSet = descriptorSets[this->descriptorset];
}
