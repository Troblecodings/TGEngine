#include "../../public/graphics/GameGraphicsModule.hpp"

#include <array>

namespace tge::graphics {

std::vector<Material> materials;

main::Error GameGraphicsModule::init() {
  apiLayer->pushMaterials(materials.size(), materials.data());
  const std::array vertData = {0.0f, 0.0f, 0.2f, 1.0f, //
                               1.0f, 0.0f, 0.2f, 1.0f, //
                               1.0f, 1.0f, 0.2f, 1.0f};
  const std::array dptr = {vertData.data()};
  const std::array size = {vertData.size() * sizeof(float)};
  apiLayer->pushData(1, (const uint8_t **)dptr.data(), size.data(),
                     DataType::VertexData);

  const std::array indexData = {0, 1, 2};
  const std::array indexdptr = {indexData.data()};
  const std::array indexsize = {indexData.size() * sizeof(int)};
  apiLayer->pushData(1, (const uint8_t **)indexdptr.data(), indexsize.data(),
                     DataType::IndexData);

  RenderInfo renderInfo;
  renderInfo.indexBuffer = 1;
  renderInfo.materialId = 0;
  renderInfo.indexCount = 3;
  renderInfo.vertexBuffer.push_back(0);
  apiLayer->pushRender(1, &renderInfo);

  return main::Error::NONE;
}

WindowProperties GameGraphicsModule::getWindowProperties() {
  return WindowProperties();
}

} // namespace tge::graphics