#pragma once

#include "../../public/Error.hpp"

namespace tge::graphics {

struct Material {
};

struct APILayer { // Interface

	virtual main::Error pushMaterials(size_t materialcount, Material* materials) = 0;

};

class GameGraphicsModule {

  const APILayer *apiLayer;

public:
  GameGraphicsModule(const APILayer *apiLayer) : apiLayer(apiLayer) {}

  main::Error init();
};

} // namespace tge::graphics