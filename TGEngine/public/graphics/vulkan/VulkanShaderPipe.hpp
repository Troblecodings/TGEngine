#pragma once

#include <vulkan/vulkan.hpp>
#include <string>
#include <vector>
#include <array>

namespace tge::shader {

using namespace vk;

struct VulkanShaderPipe {
  std::vector<std::pair<std::vector<uint32_t>, ShaderStageFlagBits>> shader;
  std::vector<PipelineShaderStageCreateInfo> pipelineShaderStage;
  std::vector<VertexInputBindingDescription> vertexInputBindings;
  std::vector<VertexInputAttributeDescription> vertexInputAttributes;
  PipelineVertexInputStateCreateInfo inputStateCreateInfo;
  PipelineRasterizationStateCreateInfo rasterization;
  std::vector<DescriptorSetLayoutBinding> descriptorLayoutBindings;
  size_t layoutID = UINT64_MAX;
};

} // namespace tge::graphics
