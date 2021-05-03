#pragma once

#include "../../public/Error.hpp"
#include "stdint.h"
#include <vector>

namespace tge::graphics {

using Color = float[4];

struct Material {
  Color color = {1, 1, 1, 1};

  uint32_t costumShaderCount = 0;      // API dependent
  uint8_t *costumShaderData = nullptr; // API dependent
};
extern std::vector<Material> materials;

class GameGraphicsModule;

struct APILayer { // Interface

  virtual main::Error pushMaterials(const size_t materialcount,
                                    const Material *materials) = 0;
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