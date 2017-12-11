#pragma once

#include "TGEngine.hpp"
#include "Device.hpp"
#include "RenderPass.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include <vector>
#include "Window.hpp"

extern VkPipeline pipeline;
extern VkPipelineLayout layout;
extern VkViewport viewport;
extern VkRect2D scissor;
extern VkDescriptorPool descriptor_pool;
extern std::vector<VkDescriptorSetLayoutBinding> descriptor_set_layout_bindings;
extern std::vector<VkPushConstantRange> push_constant_ranges;

SINCE(0, 0, 1)
void createPipeline();

SINCE(0, 0, 1)
void destroyPipeline();

SINCE(0, 0, 2)
void addDescriptor(uint32_t binding, VkDescriptorType type, uint32_t count, VkShaderStageFlags flags, uint32_t offest, uint32_t size);