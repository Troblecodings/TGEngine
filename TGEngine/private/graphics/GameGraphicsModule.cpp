#include "../../public/graphics/GameGraphicsModule.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../public/Util.hpp"
#include "../../public/graphics/VulkanGraphicsModule.hpp"
#include "../../public/headerlibs/tiny_gltf.h"
#include <array>

namespace tge::graphics {

std::vector<Material> materials;

using namespace tinygltf;

inline AddressMode gltfToAPI(int in, AddressMode def) {
  switch (in) {
  case TINYGLTF_TEXTURE_WRAP_REPEAT:
    return AddressMode::REPEAT;
  case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:
    return AddressMode::CLAMP_TO_EDGE;
  case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT:
    return AddressMode::MIRROR_REPEAT;
  }
  return def;
}

inline FilterSetting gltfToAPI(int in, FilterSetting def) {
  return (in >= 0 ? (FilterSetting)(in - TINYGLTF_TEXTURE_FILTER_NEAREST)
                  : def);
}

main::Error GameGraphicsModule::loadModel(const std::vector<uint8_t> &data,
                                          const bool binary,
                                          const std::string &baseDir) {
  TinyGLTF loader;
  std::string error;
  std::string warning;
  Model model;

  const bool rst =
      binary ? loader.LoadBinaryFromMemory(&model, &error, &warning,
                                           data.data(), data.size(), baseDir)
             : loader.LoadASCIIFromString(&model, &error, &warning,
                                          (const char *)data.data(),
                                          data.size(), baseDir);
  if (!rst) {
    printf("[GLTF][ERR]: Loading failed\n[GLTF][ERR]: %s\n[GLTF][WARN]: %s\n",
           error.c_str(), warning.c_str());
    return main::Error::GLTF_LOADER_ERROR;
  }

  if (!warning.empty()) {
    printf("[GLTF][WARN]: %s\n", warning.c_str());
  }

  std::vector<uint8_t *> ptr;
  ptr.reserve(model.buffers.size());
  std::vector<size_t> sizes;
  sizes.reserve(model.buffers.size());

  for (const auto &x : model.buffers) {
    ptr.push_back((uint8_t *)x.data.data());
    sizes.push_back(x.data.size());
  }
  const size_t dataFirstIndex =
      apiLayer->pushData(ptr.size(), (const uint8_t **)ptr.data(), sizes.data(),
                         DataType::VertexIndexData);

  size_t samplerIndex = 0;
  for (const auto &smplr : model.samplers) {
    const SamplerInfo samplerInfo = {
        gltfToAPI(smplr.minFilter, FilterSetting::LINEAR),
        gltfToAPI(smplr.minFilter, FilterSetting::LINEAR),
        gltfToAPI(smplr.wrapS, AddressMode::REPEAT),
        gltfToAPI(smplr.wrapT, AddressMode::REPEAT)};
    samplerIndex = apiLayer->pushSampler(samplerInfo);
  }
  samplerIndex -= model.samplers.size() - 1;

  for (const auto &img : model.images) {
    const auto view = img.bufferView;
    if (view >= 0) {
      throw std::runtime_error("Not supported!");
    }
  }

  std::vector<Material> materials;
  materials.reserve(model.materials.size());
  std::vector<std::string> test = {"test.vert", "test.frag"};
  const auto pipe = loadShaderPipeAndCompile(test);
  for (const auto &mat : model.materials) {
    // const auto &color = mat.pbrMetallicRoughness.baseColorFactor;
    const Material nmMat(pipe);
    materials.push_back(nmMat);
  }
  size_t materialFirstIndex =
      apiLayer->pushMaterials(materials.size(), materials.data());
  if (materialFirstIndex == -1) {
    const Material defMat(pipe);
    materialFirstIndex = apiLayer->pushMaterials(1, &defMat);
  }

  std::vector<RenderInfo> renderInfos;
  renderInfos.reserve(1000);
  for (const auto &mesh : model.meshes) {
    for (const auto &prim : mesh.primitives) {
      const auto &indexAccesor = model.accessors[prim.indices];
      const auto &indexView = model.bufferViews[indexAccesor.bufferView];
      const auto indexOffset = indexView.byteOffset + indexAccesor.byteOffset;

      std::vector<size_t> bufferIndicies;
      bufferIndicies.reserve(prim.attributes.size());
      std::vector<size_t> bufferOffsets;
      bufferOffsets.reserve(prim.attributes.size());
      for (const auto &attr : prim.attributes) {
        const auto &vertAccesor = model.accessors[attr.second];
        const auto &vertView = model.bufferViews[vertAccesor.bufferView];
        const auto vertOffset = vertView.byteOffset + vertAccesor.byteOffset;
        bufferIndicies.push_back(vertView.buffer + dataFirstIndex);
        bufferOffsets.push_back(vertOffset);
      }
      const IndexSize indextype =
          indexView.byteStride == 4 ? IndexSize::UINT32 : IndexSize::UINT16;

      const RenderInfo renderInfo = {bufferIndicies,
                                     indexView.buffer + dataFirstIndex,
                                     prim.material == -1
                                         ? materialFirstIndex
                                         : prim.material + materialFirstIndex,
                                     indexAccesor.count,
                                     1,
                                     indexOffset,
                                     indextype,
                                     bufferOffsets};

      renderInfos.push_back(renderInfo);
    }
  }
  apiLayer->pushRender(renderInfos.size(), renderInfos.data());

  return main::Error::NONE;
}

main::Error GameGraphicsModule::init() { return main::Error::NONE; }

void GameGraphicsModule::destroy() { materials.clear(); }

uint32_t GameGraphicsModule::loadTextures(
    const std::vector<std::vector<uint8_t>> &data) {
  std::vector<TextureInfo> textureInfos;

  util::OnExit onExit([&] {
    for (const auto &tex : textureInfos)
      free(tex.data);
  });

  for (const auto &dataIn : data) {
    TextureInfo info;
    info.data =
        stbi_load_from_memory(dataIn.data(), dataIn.size(), (int *)&info.width,
                              (int *)&info.height, (int *)&info.channel, 0);
    info.size = info.width * info.height * info.channel;
    textureInfos.push_back(info);
    if(info.channel == 3)
      throw std::runtime_error("Texture with 3 channels not supported!");
  }
  return apiLayer->pushTexture(textureInfos.size(), textureInfos.data());
}

uint32_t GameGraphicsModule::loadTextures(const std::vector<std::string> &names) {
  std::vector<std::vector<uint8_t>> data;
  data.reserve(names.size());
  for (const auto &name : names) {
    data.push_back(util::wholeFile(name));
  }
  return loadTextures(data);
}

} // namespace tge::graphics