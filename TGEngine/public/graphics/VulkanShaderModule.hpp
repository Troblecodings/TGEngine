#pragma once

#include "../../public/Module.hpp"
#include <string>
#include <vector>

namespace tge::graphics {

class VulkanShaderModule : public tge::main::Module {

public:
  void *loadShaderPipeAndCompile(const std::vector<std::string> &shadernames);

};

extern VulkanShaderModule *mainShaderModule;

} // namespace tge::graphics
