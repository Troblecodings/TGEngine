#include "../../public/graphics/GameGraphicsModule.hpp"

#include <array>

namespace tge::graphics {

std::vector<Material> materials;

main::Error GameGraphicsModule::init() {
  apiLayer->pushMaterials(materials.size(), materials.data());
  return main::Error::NONE;
}

WindowProperties GameGraphicsModule::getWindowProperties() {
  return WindowProperties();
}

} // namespace tge::graphics