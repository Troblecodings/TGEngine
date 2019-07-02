#include "Descriptors.hpp"

VkDescriptorPool descriptor_pool;
std::vector<VkDescriptorSet> descriptor_set;
std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
std::vector<VkDescriptorSetLayoutBinding> descriptor_bindings;

uint32_t uniform_count;
uint32_t image_sampler_count;

void addDescriptorBinding(uint32_t binding, VkDescriptorType type, VkShaderStageFlags flags) {
	descriptor_bindings.push_back({ binding, type, 1, flags });
}

void initDescriptors() {
	vlib_descriptor_pool_create_info.poolSizeCount = (uniform_count > 0 ? 1 : 0) + (image_sampler_count > 0 ? 1 : 0);
	VkDescriptorPoolSize* sizes = new VkDescriptorPoolSize[vlib_descriptor_pool_create_info.poolSizeCount];

	if(uniform_count > 0) {
		vlib_descriptor_pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		vlib_descriptor_pool_size.descriptorCount = uniform_count;
		sizes[0] = vlib_descriptor_pool_size;
	}

	if(image_sampler_count > 0) {
		vlib_descriptor_pool_size.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		vlib_descriptor_pool_size.descriptorCount = image_sampler_count;
		sizes[vlib_descriptor_pool_create_info.poolSizeCount - 1] = vlib_descriptor_pool_size;
	}

	vlib_descriptor_pool_create_info.pPoolSizes = sizes;
	lastResult = vkCreateDescriptorPool(device, &vlib_descriptor_pool_create_info, nullptr, &descriptor_pool);
	HANDEL(lastResult)

		vlib_allocate_info.descriptorPool = descriptor_pool;
}

uint32_t createDesctiptorLayout() {
	TG_VECTOR_GET_SIZE_AND_RESIZE(descriptorSetLayouts)
		lastResult = vkCreateDescriptorSetLayout(device, &vlib_descriptor_set_layout_create_info, nullptr, &descriptorSetLayouts[last_size]);
	HANDEL(lastResult)
		return (uint32_t)last_size;
}

void destroyDesctiptorLayout(uint32_t layout) {
	vkDestroyDescriptorSetLayout(device, descriptorSetLayouts[layout], nullptr);
}

void destroyDescriptorSet(uint32_t layout) {
	lastResult = vkFreeDescriptorSets(device, descriptor_pool, 1, &descriptor_set[layout]);
	HANDEL(lastResult);
}

uint32_t createDescriptorSet(uint32_t layout) {
	TG_VECTOR_GET_SIZE_AND_RESIZE(descriptor_set)

		vlib_allocate_info.pSetLayouts = &descriptorSetLayouts[layout];
	lastResult = vkAllocateDescriptorSets(device, &vlib_allocate_info, &descriptor_set[last_size]);
	HANDEL(lastResult)
		return (uint32_t)last_size;
}

void destroyDescriptors() {
	for each(VkDescriptorSetLayout var in descriptorSetLayouts) {
		vkDestroyDescriptorSetLayout(device, var, nullptr);
	}
	lastResult = vkFreeDescriptorSets(device, descriptor_pool, (uint32_t)descriptor_set.size(), descriptor_set.data());
	HANDEL(lastResult);
	descriptor_set.clear();
	descriptorSetLayouts.clear();
};

Descriptor::Descriptor(VkShaderStageFlags stage, VkDescriptorType type, uint32_t binding, uint32_t descriptorset) : shaderstage(stage), binding(binding), type(type), descriptorset(descriptorset) {
	if(this->type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
		uniform_count++;
	} else if(this->type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
		image_sampler_count++;
	}
}

Descriptor::~Descriptor() {
	// TODO Destructor

	//if (this->shaderstage == VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM) return;

	//if (this->type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
	//	uniform_count--;
	//}
	//else if (this->type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) {
	//	image_sampler_count--;
	//}
}

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
	vlib_descriptor_writes.dstSet = descriptor_set[this->descriptorset];
}
