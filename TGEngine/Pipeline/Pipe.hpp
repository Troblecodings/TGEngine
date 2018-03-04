#pragma once

#include "..\stdbase.hpp"
#include "Device.hpp"
#include "RenderPass.hpp"
#include "Shader.hpp"
#include "..\Util\Vertex.hpp"
#include <vector>
#include "Window\Window.hpp"

extern VkPipeline pipeline;
extern VkPipelineLayout layout;
extern std::vector<VkDescriptorSetLayout> descriptor_set_layout;
extern VkViewport viewport;
extern VkRect2D scissor;
extern std::vector<VkDescriptorSetLayoutBinding> descriptor_set_layout_bindings;
extern std::vector<VkPushConstantRange> push_constant_ranges;

SINCE(0, 0, 1)
void createPipeline();

SINCE(0, 0, 1)
void destroyPipeline();