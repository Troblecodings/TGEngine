#pragma once

#include "Material.hpp"
#include <string>
#include <vector>

namespace tge::shader {

enum class IOType { FLOAT, VEC2, VEC3, VEC4, MAT3, MAT4, SAMPLER2 };

struct ShaderIO {
  std::string name;
  IOType iotype;
  size_t binding;
};

enum class SamplerIOType { SAMPLER, TEXTURE };

struct SamplerIO {
  std::string name;
  SamplerIOType iotype;
  size_t binding;
};

enum class InstructionType {
    NOOP, MULTIPLY, ADD, TEXTURE, SAMPLER, SET, TEMP, VEC4CTR
};

struct Instruction {
  std::vector<size_t> inputs;
  IOType outputType;
  InstructionType instruciontType;
  std::string name;
};

enum class ShaderType { VERTEX, FRAGMENT };

struct ShaderCreateInfo {
  std::string __code;
  std::vector<ShaderIO> unifromIO;
  std::vector<ShaderIO> inputs;
  std::vector<ShaderIO> outputs;
  std::vector<SamplerIO> samplerIO;
  ShaderType shaderType;
  std::vector<Instruction> instructions;
};

using ShaderPipe = void *;

enum BindingType { UniformBuffer, Texture, Sampler };

struct BufferBindingData {
  size_t dataID;
  size_t size;
  size_t offset;
};

struct TextureBindingData {
  size_t texture;
  size_t sampler;
};

struct BindingInfo {
  size_t binding;
  size_t bindingSet;
  BindingType type;
  union BiningData {
    BufferBindingData buffer;
    TextureBindingData texture;
  } data;
};

class ShaderAPI {

public:
  virtual ~ShaderAPI() {}

  _NODISCARD virtual ShaderPipe
  loadShaderPipeAndCompile(const std::vector<std::string> &shadernames) = 0;

  _NODISCARD virtual ShaderPipe
  createShaderPipe(const ShaderCreateInfo *shaderCreateInfo,
                   const size_t shaderCount) = 0;

  _NODISCARD virtual size_t createBindings(ShaderPipe pipe,
                                           const size_t count = 1) = 0;

  virtual void bindData(const BindingInfo *info, const size_t count) = 0;

  virtual void addToRender(const size_t bindingID, void *customData) = 0;

  virtual void addToMaterial(const graphics::Material *material,
                             void *customData) = 0;

  virtual void init() = 0;

  virtual void destroy() = 0;
};

} // namespace tge::shader
