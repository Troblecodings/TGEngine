#pragma once

#include "../Stdbase.hpp"
#include "Device.hpp"
#include "../pipeline/buffer/Memory.hpp"

extern VkPipelineLayout pipelineLayout;
extern VkDescriptorSet mainDescriptorSets[3];

/*
 * This initialiazes the descriptor systems (Pool, Memory, Layouts ...)
 */
void initDescriptors();
