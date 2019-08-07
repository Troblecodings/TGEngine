#pragma once

#include "../Stdbase.hpp"
#include "Device.hpp"
#include "RenderPass.hpp"
#include "Shader.hpp"
#include <vector>
#include "Descriptors.hpp"
#include "../vlib/VulkanPipeline.hpp"

extern std::vector<VkPipeline> pipelines;

SINCE(0, 0, 1)
uint32_t createPipeline(uint32_t layout = 0);

SINCE(0, 0, 4)
void destroyPipelineLayout(uint32_t layout);


SINCE(0, 0, 4)
void destroyPipeline(uint32_t layout);

SINCE(0, 0, 1)
void destroyPipeline();