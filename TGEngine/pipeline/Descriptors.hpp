#pragma once

#include "..\Stdbase.hpp"
#include "Device.hpp"
#include "..\pipeline\buffer\Memory.hpp"

extern VkDescriptorPool descriptor_pool;
extern VkDescriptorSet descriptor_set;
extern VkDescriptorSetLayout desc_layout;
extern std::vector<VkDescriptorSetLayoutBinding> descriptor_bindings;

extern uint32_t uniform_count;
extern uint32_t image_sampler;

struct Descriptor {
	INPUT  
	VkShaderStageFlags shader_stage;
	uint32_t count;
	VkDescriptorType type;
	uint32_t buffer;

	VkSampler image_sampler;
	VkImageView image_view;
	OPT uint32_t array_index = 0;

	OUTPUT
    uint32_t binding;
	OPT VkDescriptorImageInfo desc_image_info;
	OPT VkDescriptorBufferInfo buffer_info;
};

SINCE(0, 0, 2)
void addDescriptor(Descriptor* desc);

SINCE(0, 0, 2)
void createPipelineLayout();

SINCE(0, 0, 2)
void createAllDescriptorSets();

SINCE(0, 0, 2)
void updateDescriptorSet(Descriptor* desc, uint32_t size);

SINCE(0, 0, 2)
void destroyDescriptors();
