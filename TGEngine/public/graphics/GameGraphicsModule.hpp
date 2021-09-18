#pragma once

#include "../../public/Error.hpp"
#include "../../public/Module.hpp"
#include "GameShaderModule.hpp"
#include "WindowModule.hpp"
#include "stdint.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <vector>
#include "Material.hpp"

namespace tge::graphics {

enum class IndexSize { UINT16, UINT32, NONE };

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
  uint8_t *data = nullptr;
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

enum class DataType { IndexData, VertexData, VertexIndexData, Uniform };

class APILayer : public main::Module { // Interface
protected:
  GameGraphicsModule *graphicsModule;
  shader::ShaderAPI *shaderAPI;

public:
  void setGameGraphicsModule(GameGraphicsModule *graphicsModule) {
    this->graphicsModule = graphicsModule;
  }

  virtual ~APILayer() {}
  
  _NODISCARD virtual void *loadShader(const MaterialType type) = 0; // Legacy support

  _NODISCARD virtual size_t pushMaterials(const size_t materialcount,
                                          const Material *materials) = 0;

  _NODISCARD virtual size_t pushData(const size_t dataCount,
                                     const uint8_t **data,
                                     const size_t *dataSizes,
                                     const DataType type) = 0;

  virtual void changeData(const size_t bufferIndex, const uint8_t *data,
                          const size_t dataSizes, const size_t offset = 0) = 0;

  virtual void pushRender(const size_t renderInfoCount,
                          const RenderInfo *renderInfos) = 0;

  _NODISCARD virtual size_t pushSampler(const SamplerInfo &sampler) = 0;

  _NODISCARD virtual size_t pushTexture(const size_t textureCount,
                                        const TextureInfo *textures) = 0;

  _NODISCARD GameGraphicsModule *getGraphicsModule() {
    return graphicsModule;
  };

  _NODISCARD shader::ShaderAPI *getShaderAPI() { return this->shaderAPI; }
};

struct NodeTransform {
  glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::quat rotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
};

struct NodeInfo {
  size_t material = UINT64_MAX;
  NodeTransform transforms = {};
  size_t parent = UINT64_MAX;
};

class GameGraphicsModule : public main::Module {

  APILayer *apiLayer;
  WindowModule *windowModule;

  glm::mat4 projectionMatrix;
  glm::mat4 viewMatrix;
  std::vector<glm::mat4> modelMatrices;
  std::vector<NodeTransform> node;
  std::vector<size_t> parents;
  std::vector<char> status; // jesus fuck not going to use a bool here
  bool allDirty;
  size_t dataID = UINT64_MAX;

public:
  GameGraphicsModule(APILayer *apiLayer, WindowModule *winModule);

  _NODISCARD size_t loadModel(const std::vector<char> &data, const bool binary,
                              const std::string &baseDir,
                              void *shaderPipe = nullptr);

  _NODISCARD size_t loadModel(const std::vector<char> &data,
                              const bool binary) {
    return loadModel(data, binary, "");
  }

  _NODISCARD uint32_t loadTextures(const std::vector<std::vector<char>> &data);

  _NODISCARD uint32_t loadTextures(const std::vector<std::string> &names);

  _NODISCARD size_t addNode(const NodeInfo *nodeInfos, const size_t count);

  void updateTransform(const size_t nodeID, const NodeTransform &transform);

  main::Error init() override;

  void tick(double time) override;

  void destroy() override;

  _NODISCARD APILayer *getAPILayer() { return apiLayer; }

  _NODISCARD WindowModule *getWindowModule() { return windowModule; }
};

} // namespace tge::graphics