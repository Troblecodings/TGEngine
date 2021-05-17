#include "../../public/graphics/GameGraphicsModule.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../public/headerlibs/tiny_gltf.h"
#include <array>

namespace tge::graphics {

std::vector<Material> materials;

using namespace tinygltf;

main::Error GameGraphicsModule::loadModel(const uint8_t *bytes,
                                          const size_t size, const bool binary,
                                          const std::string &baseDir) {
  TinyGLTF loader;
  std::string error;
  std::string warning;
  Model model;

  const bool rst =
      binary ? loader.LoadBinaryFromMemory(&model, &error, &warning, bytes,
                                           size, baseDir)
             : loader.LoadASCIIFromString(&model, &error, &warning,
                                          (const char *)bytes, size, baseDir);
  if (!rst) {
    printf("[GLTF][ERR]: Loading failed\n[GLTF][ERR]: %s\n[GLTF][WARN]: %s\n",
           error.c_str(), warning.c_str());
    return main::Error::GLTF_LOADER_ERROR;
  }

  return main::Error::NONE;
}

main::Error GameGraphicsModule::init() { return main::Error::NONE; }

void GameGraphicsModule::destroy() { materials.clear(); }

WindowProperties GameGraphicsModule::getWindowProperties() {
  return WindowProperties();
}

} // namespace tge::graphics