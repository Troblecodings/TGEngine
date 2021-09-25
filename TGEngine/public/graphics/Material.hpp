#pragma once

#include <stdint.h>

namespace tge::graphics {

class APILayer;

using Color = float[4];

enum class MaterialType { None, TextureOnly };
constexpr MaterialType MAX_TYPE = MaterialType::TextureOnly;

struct TextureMaterial {
  uint32_t textureIndex;
  uint32_t samplerIndex;
};

struct Material {

  Material(const TextureMaterial texture, APILayer *layer);

  Material(void *costumShaderData) : costumShaderData(costumShaderData) {}

  Material() = default;

  MaterialType type = MaterialType::None;
  union data {
    TextureMaterial textureMaterial;
  } data;
  void *costumShaderData = nullptr; // API dependent
  bool doubleSided = false;
};
} // namespace tge::graphics
