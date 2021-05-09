#include "../../public/graphics/GameGraphicsModule.hpp"

#include <array>

namespace tge::graphics {

std::vector<Material> materials;

main::Error GameGraphicsModule::init() {
  apiLayer->pushMaterials(materials.size(), materials.data());
  const std::array vertData = {0.0f, 0.0f, 0.0f, 1.0f, //
                               1.0f, 0.0f, 0.0f, 1.0f, //
                               1.0f, 1.0f, 0.0f, 1.0f};
  const auto dptr = vertData.data();
  const auto size = vertData.size() * sizeof(float);
  apiLayer->pushVertexData(1, (uint8_t **)(&dptr), &size);
  return main::Error::NONE;
}

WindowProperties GameGraphicsModule::getWindowProperties() {
  return WindowProperties();
}

} // namespace tge::graphics