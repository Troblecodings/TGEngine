#pragma once

#include "../../public/Stdbase.hpp"
#include "../../public/pipeline/Device.hpp"
#include "../../public/pipeline/buffer/Memory.hpp"

extern VkPipelineLayout pipelineLayout;
extern VkDescriptorSet mainDescriptorSets[3];
extern VkDescriptorSetLayout descriptorSetLayout[2];
extern VkDescriptorPool descriptorPool;

/*
 * This initialiazes the descriptor systems (Pool, Memory, Layouts ...)
 */
void initDescriptors();

/*
 * This will clean up the descriptor systems (Pool, Memory, Layouts ...)
 */
void destroyDescriptor();
