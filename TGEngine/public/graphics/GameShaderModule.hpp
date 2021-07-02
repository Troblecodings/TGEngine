#pragma once

#include <string>

namespace tge::shader {

struct UniformInputs {
  std::string name;
};

struct VarInOuts {
  std::string name;
};

enum class ShaderType { VERTEX, FRAGMENT };

struct ShaderCreateInfo {
  char *code;
  size_t codeSize;
  UniformInputs *uniformIn;
  size_t uniformSize;
  VarInOuts *varInOuts;
  size_t varInOutSize;
  ShaderType shaderType;
};

class ShaderAPI {

  virtual void *createShaderPipe(const ShaderCreateInfo *shaderCreateInfo,
                                 const size_t shaderCount) = nullptr;
};

} // namespace tge::shader
