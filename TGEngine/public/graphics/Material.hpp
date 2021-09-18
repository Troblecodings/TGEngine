#pragma once

namespace tge::graphics {

using Color = float[4];

enum class MaterialType { None, TextureOnly };
constexpr MaterialType MAX_TYPE = MaterialType::TextureOnly;

struct TextureMaterial {
  uint32_t textureIndex;
  uint32_t samplerIndex;
};

struct Material {

  Material(const TextureMaterial texture, APILayer *layer)
      : type(MaterialType::TextureOnly), data({texture}) {
    costumShaderData = layer->loadShader(type);
  }

  Material(void *costumShaderData) : costumShaderData(costumShaderData) {}

  Material() = default;

  MaterialType type = MaterialType::None;
  union data {
    TextureMaterial textureMaterial;
  } data;
  void *costumShaderData = nullptr; // API dependent
};
} // namespace tge::graphics
