#pragma once

#include "../Stdbase.hpp"
#include "Device.hpp"
#include "../pipeline/buffer/Memory.hpp"

extern VkPipelineLayout pipelineLayout;
extern VkDescriptorSet mainDescriptorSets[3];
extern VkDescriptorSetLayout descriptorSetLayout[2];
extern VkDescriptorPool descriptorPool;

/*
 * This initialiazes the descriptor systems (Pool, Memory, Layouts ...)
 */
void initDescriptors();

/*
 * This will cleanup the descriptor systems (Pool, Memory, Layouts ...)
 */
void destroyDescriptor();
