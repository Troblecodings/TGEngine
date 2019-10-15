#pragma once

#include "../Stdbase.hpp"
#include "Device.hpp"
#include "../pipeline/buffer/Memory.hpp"
#include "../vlib/VulkanDescriptor.hpp"

extern VkPipelineLayout pipelineLayout;
extern VkDescriptorSet mainDescriptorSet;

/*
 * This initialiazes the descriptor systems (Pool, Memory, Layouts ...)
 */
void initDescriptors();
