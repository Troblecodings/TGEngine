#pragma once

#include "../GameShaderModule.hpp"
#include "VulkanShaderPipe.hpp"
#include <vulkan/vulkan.hpp>

namespace tge::shader {

struct BindingPipeInfo {
  size_t descSet;
  size_t pipeline;
};

class VulkanShaderModule : public tge::shader::ShaderAPI {

public:
  explicit VulkanShaderModule(void *vgm) : vgm(vgm) {}

  void *vgm;
  std::vector<vk::DescriptorPool> descPools;
  std::vector<vk::PipelineLayout> pipeLayouts;
  std::vector<vk::DescriptorSetLayout> setLayouts;
  std::vector<vk::DescriptorSet> descSets;
  std::vector<BindingPipeInfo> pipeInfos;
  std::vector<tge::shader::VulkanShaderPipe *> shaderPipes;
  DescriptorSetLayout defaultDescLayout;
  PipelineLayout defaultLayout;
  // Legacy support
  std::vector<std::vector<BindingInfo>> defaultbindings;

  ShaderPipe loadShaderPipeAndCompile(
      const std::vector<std::string> &shadernames) override;

  ShaderPipe createShaderPipe(const ShaderCreateInfo *shaderCreateInfo,
                              const size_t shaderCount) override;

  size_t createBindings(ShaderPipe pipe, const size_t count = 1) override;

  void changeInputBindings(const ShaderPipe pipe, const size_t bindingID,
                           const size_t buffer) override;

  void bindData(const BindingInfo *info, const size_t count) override;

  void addToRender(const size_t* bindingID, const size_t size, void *customData) override;

  void addToMaterial(const graphics::Material *material,
                     void *customData) override;

  void init() override;

  void destroy() override;
};

} // namespace tge::shader
