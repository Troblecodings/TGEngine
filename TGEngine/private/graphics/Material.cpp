#include "../../public/graphics/Material.hpp"
#include "../../public/graphics/APILayer.hpp"

namespace tge::graphics {
tge::graphics::Material::Material(const TextureMaterial texture,
                                  APILayer *layer)
    : type(MaterialType::TextureOnly), data({texture}) {
  costumShaderData = layer->loadShader(type);
}
} // namespace tge::graphics