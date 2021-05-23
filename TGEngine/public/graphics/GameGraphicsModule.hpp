#pragma once

#include "../../public/Error.hpp"
#include "../../public/Module.hpp"
#include "WindowModule.hpp"
#include "stdint.h"
#include <string>
#include <vector>

namespace tge::graphics {

using Color = float[4];

struct Material {
  Color color = {1, 1, 1, 1};

  uint8_t *costumShaderData = nullptr; // API dependent
};

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

  const GameGraphicsModule *getGraphicsModule() { return graphicsModule; };
};

extern std::vector<Material> materials;

class GameGraphicsModule : public main::Module {

  APILayer *apiLayer;
  WindowModule *windowModule;

public:
  GameGraphicsModule(APILayer *apiLayer, WindowModule *winModule)
      : apiLayer(apiLayer), windowModule(winModule) {}

  main::Error loadModel(const uint8_t *bytes, const size_t size,
                        const bool binary, const std::string &baseDir);

  main::Error loadModel(const uint8_t *bytes, const size_t size,
                        const bool binary) {
    return loadModel(bytes, size, binary, "");
  }

  main::Error init() override;

  void destroy() override;

  APILayer *getAPILayer() { return apiLayer; }

  WindowModule *getWindowModule() { return windowModule; }
};

} // namespace tge::graphics