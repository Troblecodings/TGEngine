#pragma once

#include "../Stdbase.hpp"
#include "Device.hpp"
#include "../pipeline/buffer/Memory.hpp"
#include "../vlib/VulkanDescriptor.hpp"

extern VkDescriptorPool descriptor_pool;
extern std::vector<VkDescriptorSet> descriptor_set;
extern std::vector<VkDescriptorSetLayout> descriptor_set_layouts;
extern std::vector<VkDescriptorSetLayoutBinding> descriptor_bindings;

extern uint32_t uniform_count;
extern uint32_t image_sampler;

struct Descriptor {
	INPUT  
	VkShaderStageFlags shader_stage = VK_SHADER_STAGE_VERTEX_BIT;
	uint32_t count = 1;
	VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uint32_t buffer = 0;

	VkSampler image_sampler;
	VkImageView* image_view;
	OPT uint32_t array_index = 0;
	OPT size_t descriptor_set = 0;

	OUTPUT
    uint32_t binding;
	OPT VkDescriptorImageInfo* desc_image_info;
	OPT VkDescriptorBufferInfo buffer_info;
};

SINCE(0, 0, 2)
void addDescriptor(Descriptor* desc);

SINCE(0, 0, 4)
void initDescriptors();

SINCE(0, 0, 4)
void createDesctiptorLayout();

SINCE(0, 0, 4)
void createDescriptorSet();

SINCE(0, 0, 2)
void updateDescriptorSet(Descriptor* desc, uint32_t size);

SINCE(0, 0, 2)
void destroyDescriptors();
