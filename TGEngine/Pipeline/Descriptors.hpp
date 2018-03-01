#pragma once

#include "..\stdbase.hpp"
#include "Device.hpp"
#include "..\Pipeline\Buffer\Memory.hpp"
#include "Pipe.hpp"

extern VkDescriptorPool descriptor_pool;
extern std::vector<VkDescriptorSet> descriptor_sets;

struct Descriptor {
	INPUT  
	uint32_t buffer;
	uint32_t count;
	VkDescriptorType type;
	VkShaderStageFlags shader_stage;
	OUTPUT 
    uint32_t binding;
	uint32_t descriptor;
};

SINCE(0, 0, 2)
void addUniformBuffer(Descriptor* buffer);

SINCE(0, 0, 2)
void createAllDescriptorSets();

SINCE(0, 0, 2)
void destroyDescriptors();

SINCE(0, 0, 2)
void updateDescriptorSet(Descriptor* desc, uint32_t size);

SINCE(0, 0, 2)
void addDescriptor(Descriptor* desc);