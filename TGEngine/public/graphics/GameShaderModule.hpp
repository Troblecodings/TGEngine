#pragma once

#include <string>
#include <vector>

namespace tge::shader {

struct ShaderIO {
  std::string name;
  size_t size;
};

enum class ShaderType { VERTEX, FRAGMENT };

struct ShaderCreateInfo {
  std::vector<char> code;
  std::vector<ShaderIO> unifromIO;
  std::vector<ShaderIO> inputs;
  std::vector<ShaderIO> outputs;
  ShaderType shaderType;
};

using ShaderPipe = void *;

class ShaderAPI {

public:
  virtual ~ShaderAPI() {}

  virtual ShaderPipe
  loadShaderPipeAndCompile(const std::vector<std::string> &shadernames) = 0;

  virtual ShaderPipe createShaderPipe(const ShaderCreateInfo *shaderCreateInfo,
                                      const size_t shaderCount) = 0;
};

} // namespace tge::shader
