#pragma once

#include <vulkan/vulkan.hpp>

namespace tge::graphics {

using namespace vk;

_CONSTEXPR20_CONTAINER std::string operator""_str(const char *chr,
                                                  std::size_t size) {
  return std::string(chr, size);
}

_CONSTEXPR20_CONTAINER std::array shaderNames = {
    std::vector({"assets/testvec4.vert"_str, "assets/test.frag"_str}),
    std::vector({"assets/testUV.vert"_str, "assets/testTexture.frag"_str})};

struct VulkanShaderPipe {
  std::vector<std::pair<std::vector<uint32_t>, ShaderStageFlagBits>> shader;
  std::vector<PipelineShaderStageCreateInfo> pipelineShaderStage;
  std::vector<VertexInputBindingDescription> vertexInputBindings;
  std::vector<VertexInputAttributeDescription> vertexInputAttributes;
  PipelineVertexInputStateCreateInfo inputStateCreateInfo;
  PipelineRasterizationStateCreateInfo rasterization;
  std::vector<DescriptorSetLayoutCreateInfo> descriptorLayout;
  std::vector<std::vector<DescriptorSetLayoutBinding>> descriptorLayoutBindings;
};

} // namespace tge::graphics
