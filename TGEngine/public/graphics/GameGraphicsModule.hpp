#pragma once

#include "../../public/Error.hpp"
#include "../../public/Module.hpp"
#include "WindowModule.hpp"
#include "stdint.h"
#include <string>
#include <vector>

namespace tge::graphics {

using Color = float[4];

enum class MaterialType {
    None,
    TextureOnly
};

struct TextureMaterial {
  uint32_t textureIndex;
  uint32_t samplerIndex;
};

struct Material;

enum class IndexSize { UINT16, UINT32 };

struct RenderInfo {
  std::vector<size_t> vertexBuffer;
  size_t indexBuffer;
  size_t materialId;
  size_t indexCount;
  size_t instanceCount = 1;
  size_t indexOffset = 0;
  IndexSize indexSize = IndexSize::UINT32;
  std::vector<size_t> vertexOffsets;
};

struct TextureInfo {
  uint8_t* data = nullptr;
  uint32_t size;
  uint32_t width;
  uint32_t height;
  uint32_t channel;
};

enum class FilterSetting { NEAREST, LINEAR };

enum class AddressMode {
  REPEAT,
  MIRROR_REPEAT,
  CLAMP_TO_EDGE,
  CLAMP_TO_BORDER,
  MIRROR_CLAMP_TO_EDGE
};

struct SamplerInfo {
  FilterSetting minFilter;
  FilterSetting magFilter;
  AddressMode uMode;
  AddressMode vMode;
  int anisotropy = 0;
};

class GameGraphicsModule;

enum class DataType { IndexData, VertexData, VertexIndexData };

class APILayer : public main::Module { // Interface
protected:
  GameGraphicsModule *graphicsModule;

public:
  void setGameGraphicsModule(GameGraphicsModule *graphicsModule) {
    this->graphicsModule = graphicsModule;
  }

  virtual ~APILayer() {}

  virtual size_t pushMaterials(const size_t materialcount,
                               const Material *materials) = 0;

  virtual size_t pushData(const size_t dataCount, const uint8_t **data,
                          const size_t *dataSizes, const DataType type) = 0;

  virtual void pushRender(const size_t renderInfoCount,
                          const RenderInfo *renderInfos) = 0;

  virtual size_t pushSampler(const SamplerInfo &sampler) = 0;

  virtual size_t pushTexture(const size_t textureCount,
                             const TextureInfo *textures) = 0;

  virtual void *loadShader(const MaterialType type) = 0;

  const GameGraphicsModule *getGraphicsModule() { return graphicsModule; };
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

class GameGraphicsModule : public main::Module {

  APILayer *apiLayer;
  WindowModule *windowModule;

public:
  GameGraphicsModule(APILayer *apiLayer, WindowModule *winModule)
      : apiLayer(apiLayer), windowModule(winModule) {}

  main::Error loadModel(const std::vector<uint8_t> &data,
                        const bool binary, const std::string &baseDir);

  main::Error loadModel(const std::vector<uint8_t> &data,
                        const bool binary) {
    return loadModel(data, binary, "");
  }

  uint32_t loadTextures(const std::vector<std::vector<uint8_t>> &data);

  uint32_t loadTextures(const std::vector<std::string> &names);

  main::Error init() override;

  void destroy() override;

  APILayer *getAPILayer() { return apiLayer; }

  WindowModule *getWindowModule() { return windowModule; }
};

} // namespace tge::graphics