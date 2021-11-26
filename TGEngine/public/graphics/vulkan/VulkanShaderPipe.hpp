#pragma once

#include <vulkan/vulkan.hpp>
#include <string>
#include <vector>
#include <array>

namespace tge::shader {

using namespace vk;
using namespace std::string_literals;

const std::array shaderNames = {
    std::vector({"assets/testvec4.vert"s, "assets/test.frag"s}),
    std::vector({"assets/testUV.vert"s, "assets/testTexture.frag"s})};

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
