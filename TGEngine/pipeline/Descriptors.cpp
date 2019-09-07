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

// TODO switch back to global texture binding for 3D 2D and so on ... map on same binding

void initDescriptors() {
	vlibDescriptorPoolCreateInfo.poolSizeCount = 2;
	VkDescriptorPoolSize* sizes = new VkDescriptorPoolSize[vlibDescriptorPoolCreateInfo.poolSizeCount];

	vlibDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	vlibDescriptorPoolSize.descriptorCount = TGE_MIN(deviceProperties.limits.maxDescriptorSetUniformBuffers, 100);
	sizes[0] = vlibDescriptorPoolSize;

	vlibDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	vlibDescriptorPoolSize.descriptorCount = TGE_MIN(deviceProperties.limits.maxDescriptorSetSamplers, 100);
	sizes[1] = vlibDescriptorPoolSize;

	vlibDescriptorPoolCreateInfo.pPoolSizes = sizes;
	lastResult = vkCreateDescriptorPool(device, &vlibDescriptorPoolCreateInfo, nullptr, &descriptor_pool);
	HANDEL(lastResult)

	vlibAllocateInfo.descriptorPool = descriptor_pool;
}

uint32_t createLayouts() {
	TG_VECTOR_GET_SIZE_AND_RESIZE(pipeLayouts)
	TG_VECTOR_GET_SIZE_AND_RESIZE(descriptorSetLayouts)

	lastResult = vkCreateDescriptorSetLayout(device, &vlibDescriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayouts[lastSize]);
	HANDEL(lastResult)

	vlibLayoutInfo.pSetLayouts = &descriptorSetLayouts[lastSize];
	lastResult = vkCreatePipelineLayout(device, &vlibLayoutInfo, nullptr, &pipeLayouts[lastSize]);
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

		vlibAllocateInfo.pSetLayouts = &descriptorSetLayouts[layout];
	lastResult = vkAllocateDescriptorSets(device, &vlibAllocateInfo, &descriptorSets[lastSize]);
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
};

void Descriptor::updateImageInfo(VkSampler sampler, VkImageView view) {
	if(this->type != VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) return;
	update();

	VkDescriptorImageInfo imageinfo;
	imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageinfo.imageView = view;
	imageinfo.sampler = sampler;

	vlibDescriptorWrites.pBufferInfo = nullptr;
	vlibDescriptorWrites.pImageInfo = &imageinfo;

	vkUpdateDescriptorSets(device, 1, &vlibDescriptorWrites, 0, nullptr);
}

void Descriptor::updateBufferInfo(uint32_t buffer, size_t size) {
	if(this->type != VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) return;
	update();

	VkDescriptorBufferInfo bufferinfo;
	bufferinfo.buffer = buffers[buffer];
	bufferinfo.range = size;
	bufferinfo.offset = 0;

	vlibDescriptorWrites.pBufferInfo = &bufferinfo;
	vlibDescriptorWrites.pImageInfo = nullptr;

	vkUpdateDescriptorSets(device, 1, &vlibDescriptorWrites, 0, nullptr);
}

void Descriptor::update() {
	vlibDescriptorWrites.descriptorType = this->type;
	vlibDescriptorWrites.dstArrayElement = 0;
	vlibDescriptorWrites.dstBinding = this->binding;
	vlibDescriptorWrites.descriptorCount = 1;
	vlibDescriptorWrites.dstSet = descriptorSets[this->descriptorset];
}
