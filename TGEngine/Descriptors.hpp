#pragma once

#include "Pipeline.hpp"

extern VkDescriptorPool descriptor_pool;
extern std::vector<VkDescriptorSet> descriptor_sets;

SINCE(0, 0, 2)
void createDescriptorsForUniformBuffers(std::vector<uint32_t> descriptors);

SINCE(0, 0, 2)
void destroyDescriptors();

SINCE(0, 0, 2)
void updateDescriptorSet(uint32_t index, uint32_t binding, uint32_t size);

SINCE(0, 0, 2)
uint32_t getDescriptorIndex(uint32_t buffer_index);