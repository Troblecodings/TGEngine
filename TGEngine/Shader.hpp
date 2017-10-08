#pragma once

#include <vector>
#include "TGEngine.hpp"
#include "Files.hpp"
#include "Device.hpp"

extern std::vector<VkPipelineShaderStageCreateInfo> shaders;
extern std::vector<VkVertexInputAttributeDescription> description_attributes;

SINCE(0, 0, 1)
void createShader(char* shader, VkShaderStageFlagBits flag);

SINCE(0, 0, 1)
void createShaderInput(uint32_t location, uint32_t offset, VkFormat format);