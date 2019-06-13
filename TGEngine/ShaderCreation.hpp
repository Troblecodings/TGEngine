#pragma once

#include "Stdbase.hpp"
#include "pipeline/Device.hpp"

VkPipelineShaderStageCreateInfo createShader(unsigned char data[], VkShaderStageFlagBits flag, uint32_t size);