#pragma once

#include "..\stdbase.hpp"
#include "Device.hpp"
#include "..\Pipeline\Buffer\Memory.hpp"

extern VkDescriptorPool descriptor_pool;
extern std::vector<VkDescriptorSet> descriptor_sets;
extern std::vector<VkDescriptorSetLayout> descriptor_set_layouts;

struct Descriptor {
	INPUT  
	uint32_t count;
	VkDescriptorType type;
	VkShaderStageFlags shader_stage;
	uint32_t buffer;
	OUTPUT
    uint32_t binding;
};

SINCE(0, 0, 2)
void addDescriptor(Descriptor* desc);

SINCE(0, 0, 2)
void createAllDescriptorSets();

SINCE(0, 0, 2)
void updateDescriptorSet(Descriptor* desc, uint32_t size);

SINCE(0, 0, 2)
void destroyDescriptors();
