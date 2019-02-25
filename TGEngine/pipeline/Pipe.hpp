#pragma once

#include "../Stdbase.hpp"
#include "Device.hpp"
#include "RenderPass.hpp"
#include "Shader.hpp"
#include <vector>
#include "window/Window.hpp"
#include "Descriptors.hpp"
#include "../vlib/VulkanPipeline.hpp"

extern std::vector<VkPipeline> pipelines;
extern std::vector<VkPipelineLayout> layouts;

SINCE(0, 0, 1)
void createPipeline();

SINCE(0, 0, 1)
void destroyPipeline();