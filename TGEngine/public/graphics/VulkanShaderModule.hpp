#pragma once

#include "../../public/graphics/GameShaderModule.hpp"

namespace tge::shader {

class VulkanShaderModule : public tge::shader::ShaderAPI {

public:
  ShaderPipe loadShaderPipeAndCompile(const std::vector<std::string> &shadernames) override;

  ShaderPipe createShaderPipe(const ShaderCreateInfo *shaderCreateInfo,
                                      const size_t shaderCount) override;
};

} // namespace tge::graphics
