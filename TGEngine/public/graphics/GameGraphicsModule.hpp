#pragma once

#include "../../public/Error.hpp"
#include "stdint.h"
#include <vector>

namespace tge::graphics {

using Color = float[4];

struct Material {
  Color color = {1, 1, 1, 1};

  uint8_t *costumShaderData = nullptr; // API dependent
};

struct RenderInfo {
  std::vector<uint32_t> vertexBuffer;
  uint32_t indexBuffer;
  uint32_t materialId;
  uint32_t indexCount;
  uint32_t instanceCount = 1;
};

extern std::vector<Material> materials;

class GameGraphicsModule;

enum class DataType { IndexData, VertexData };

struct APILayer { // Interface

  virtual main::Error pushMaterials(const size_t materialcount,
                                    const Material *materials) = 0;

  virtual main::Error pushData(const size_t dataCount, const uint8_t **data,
                               const size_t *dataSizes,
                               const DataType type) = 0;

  virtual main::Error pushRender(const size_t renderInfoCount, const RenderInfo* renderInfos) = 0;

  virtual GameGraphicsModule *getGraphicsModule() = 0;
};

struct WindowProperties {
  bool centered = false;   // Not implemented
  int x = 0;               // Ignored if centered
  int y = 0;               // Ignored if centered
  char fullscreenmode = 0; // Not implemented
  int width = 800;         // Ignored if fullscreenmode != 0
  int height = 800;        // Ignored if fullscreenmode != 0
};

class GameGraphicsModule {

  APILayer *apiLayer;

public:
  GameGraphicsModule(APILayer *apiLayer) : apiLayer(apiLayer) {}

  main::Error init();

  WindowProperties getWindowProperties();
};

} // namespace tge::graphics