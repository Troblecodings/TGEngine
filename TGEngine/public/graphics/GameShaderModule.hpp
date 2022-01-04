#pragma once

#include "Material.hpp"
#include <string>
#include <vector>
#undef ERROR
#include "../headerlibs/json.hpp"
#define SPR_NO_JSON_HPP_INCLUDE 1
#include <ShaderPermute.hpp>

namespace tge::shader {

using ShaderCreateInfo = permute::Permute<permute::PermuteGLSL>;

using ShaderPipe = void *;

enum BindingType { UniformBuffer, Texture, Sampler, InputAttachment };

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
  size_t arrayID = 0;
};

class ShaderAPI {

public:
  virtual ~ShaderAPI() {}

  _NODISCARD virtual ShaderPipe loadShaderPipeAndCompile(
      const std::vector<std::string> &shadernames,
      const std::vector<std::string> &dependencies = {}) = 0;

  _NODISCARD virtual ShaderPipe
  createShaderPipe(ShaderCreateInfo *shaderCreateInfo,
                   const size_t shaderCount,
                   const std::vector<std::string> &dependencies = {}) = 0;

  _NODISCARD virtual size_t createBindings(ShaderPipe pipe,
                                           const size_t count = 1) = 0;

  virtual void changeInputBindings(const ShaderPipe pipe,
                                   const size_t bindingID,
                                   const size_t buffer) = 0;

  virtual void updateAllTextures() = 0;

  virtual void bindData(const BindingInfo *info, const size_t count) = 0;

  virtual void addToRender(const size_t *bindingID, const size_t size,
                           void *customData) = 0;

  virtual void addToMaterial(const graphics::Material *material,
                             void *customData) = 0;

  virtual void init() = 0;

  virtual void destroy() = 0;
};

} // namespace tge::shader
