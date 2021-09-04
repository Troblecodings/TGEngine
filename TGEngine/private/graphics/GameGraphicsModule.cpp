#include "../../public/graphics/GameGraphicsModule.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../public/Util.hpp"
#include "../../public/graphics/VulkanGraphicsModule.hpp"
#include "../../public/graphics/VulkanShaderPipe.hpp"
#include "../../public/headerlibs/tiny_gltf.h"
#include <array>
#include <iostream>

namespace tge::graphics {

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
  switch (in) {
  case TINYGLTF_TEXTURE_FILTER_LINEAR:
    return FilterSetting::LINEAR;
  case TINYGLTF_TEXTURE_FILTER_NEAREST:
    return FilterSetting::NEAREST;
  default:
    return def;
  }
}

inline vk::Format getFormatFromStride(uint32_t stride) {
  switch (stride) {
  case 4:
    return vk::Format::eR8Unorm;
  case 8:
    return vk::Format::eR8G8Unorm;
  case 12:
    return vk::Format::eR8G8B8Unorm;
  case 16:
    return vk::Format::eR8G8B8A8Unorm;
  default:
    throw std::runtime_error("Couldn't find format");
  }
}

main::Error GameGraphicsModule::loadModel(const std::vector<char> &data,
                                          const bool binary,
                                          const std::string &baseDir,
                                          void *shaderPipe) {
  TinyGLTF loader;
  std::string error;
  std::string warning;
  Model model;

  const bool rst =
      binary ? loader.LoadBinaryFromMemory(&model, &error, &warning,
                                           (const uint8_t *)data.data(),
                                           data.size(), baseDir)
             : loader.LoadASCIIFromString(&model, &error, &warning, data.data(),
                                          data.size(), baseDir);
  if (!rst) {
    printf("[GLTF][ERR]: Loading failed\n[GLTF][ERR]: %s\n[GLTF][WARN]: %s\n",
           error.c_str(), warning.c_str());
    return main::Error::GLTF_LOADER_ERROR;
  }

  if (!warning.empty()) {
    printf("[GLTF][WARN]: %s\n", warning.c_str());
  }

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

  std::vector<TextureInfo> textureInfos;

  for (const auto &img : model.images) {
    if (!img.image.empty()) {
      const TextureInfo info{(uint8_t *)img.image.data(),
                             (uint32_t)img.image.size(), (uint32_t)img.width,
                             (uint32_t)img.height, (uint32_t)img.component};
      textureInfos.push_back(info);
    }
  }

  if (!textureInfos.empty()) {
    apiLayer->pushTexture(textureInfos.size(), textureInfos.data());
    if (model.samplers.empty()) { // default sampler
      const SamplerInfo samplerInfo = {
          FilterSetting::LINEAR, FilterSetting::LINEAR, AddressMode::REPEAT,
          AddressMode::REPEAT};
      samplerIndex = apiLayer->pushSampler(samplerInfo);
    }
  }

  std::vector<RenderInfo> renderInfos;
  renderInfos.reserve(1000);

  std::vector<uint8_t *> ptr;
  ptr.reserve(model.buffers.size());
  std::vector<size_t> sizes;
  sizes.reserve(model.buffers.size());

  auto pipe = (tge::shader::VulkanShaderPipe *)(shaderPipe == nullptr
                                                    ? apiLayer->loadShader(
                                                          MaterialType::None)
                                                    : shaderPipe);
  pipe->vertexInputAttributes.clear();
  pipe->vertexInputBindings.clear();

  const size_t dataFirstIndex = 0;     // TODO querry
  const size_t materialFirstIndex = 0; // TODO querry

  for (const auto &mesh : model.meshes) {
    for (const auto &prim : mesh.primitives) {
      if (prim.indices >= 0) [[likely]] {
        const auto &indexAccesor = model.accessors[prim.indices];
        const auto &indexView = model.bufferViews[indexAccesor.bufferView];
        const auto &indexBuffer = model.buffers[indexView.buffer];
        ptr.push_back((uint8_t *)indexBuffer.data.data());
        sizes.push_back(indexBuffer.data.size());
      }

      std::vector<std::tuple<int, int, int>> strides;
      strides.reserve(prim.attributes.size());

      for (const auto &attr : prim.attributes) {
        const auto &vertAccesor = model.accessors[attr.second];
        const auto &vertView = model.bufferViews[vertAccesor.bufferView];
        const auto &vertBuffer = model.buffers[vertView.buffer];
        const auto bufferID = vertView.buffer + dataFirstIndex;
        const auto vertOffset = vertView.byteOffset + vertAccesor.byteOffset;
        strides.push_back(std::make_tuple(vertAccesor.ByteStride(vertView),
                                          bufferID, vertOffset));

        const auto ptrto = (uint8_t *)vertBuffer.data.data();
        if (std::find(ptr.begin(), ptr.end(), ptrto) != ptr.end())
          continue;
        ptr.push_back(ptrto);
        sizes.push_back(vertBuffer.data.size());
      }

      std::vector<size_t> bufferIndicies;
      bufferIndicies.reserve(prim.attributes.size());
      std::vector<size_t> bufferOffsets;
      bufferOffsets.reserve(prim.attributes.size());
      std::sort(strides.begin(), strides.end(),
                [](auto x, auto y) { return std::get<0>(x) > std::get<0>(y); });
      uint32_t location = 0;
      for (auto &stride : strides) {
        const auto strid = std::get<0>(stride);
        std::cout << strid << std::endl;
        pipe->vertexInputAttributes.push_back(
            vk::VertexInputAttributeDescription(location, location,
                                                getFormatFromStride(strid)));
        pipe->vertexInputBindings.push_back(
            vk::VertexInputBindingDescription(location, strid));
        location++;

        bufferIndicies.push_back(std::get<1>(stride));
        bufferOffsets.push_back(std::get<2>(stride));
      }

      if (prim.indices >= 0) [[likely]] {
        const auto &indexAccesor = model.accessors[prim.indices];
        const auto &indexView = model.bufferViews[indexAccesor.bufferView];
        const auto indexOffset = indexView.byteOffset + indexAccesor.byteOffset;
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
      } else {
        const auto accessorID = prim.attributes.begin()->second;
        const auto &vertAccesor = model.accessors[accessorID];
        const RenderInfo renderInfo = {bufferIndicies,
                                       0,
                                       prim.material == -1
                                           ? materialFirstIndex
                                           : prim.material + materialFirstIndex,
                                       0,
                                       1,
                                       vertAccesor.count,
                                       IndexSize::NONE,
                                       bufferOffsets};
        renderInfos.push_back(renderInfo);
      }
    }
  }
  pipe->inputStateCreateInfo = vk::PipelineVertexInputStateCreateInfo(
      {}, pipe->vertexInputBindings, pipe->vertexInputAttributes);
  apiLayer->pushData(ptr.size(), (const uint8_t **)ptr.data(), sizes.data(),
                     DataType::VertexIndexData);

  std::vector<Material> materials;
  materials.reserve(model.materials.size());
  for (const auto &mat : model.materials) {
    // const auto &color = mat.pbrMetallicRoughness.baseColorFactor;
    Material nmMat(pipe);
    nmMat.type = MaterialType::TextureOnly;
    nmMat.data.textureMaterial.samplerIndex = samplerIndex;
    nmMat.data.textureMaterial.textureIndex =
        mat.pbrMetallicRoughness.baseColorTexture.index;
    materials.push_back(nmMat);
  }

  if (materials.empty()) {
    const Material defMat(pipe);
    apiLayer->pushMaterials(1, &defMat);
  } else {
    apiLayer->pushMaterials(materials.size(), materials.data());
  }
  apiLayer->pushRender(renderInfos.size(), renderInfos.data());

  return main::Error::NONE;
}

main::Error GameGraphicsModule::init() { return main::Error::NONE; }

void GameGraphicsModule::destroy() {}

uint32_t
GameGraphicsModule::loadTextures(const std::vector<std::vector<char>> &data) {
  std::vector<TextureInfo> textureInfos;

  util::OnExit onExit([tinfos = &textureInfos] {
    for (const auto &tex : *tinfos)
      if (tex.data != nullptr)
        free(tex.data);
  });

  for (const auto &dataIn : data) {
    TextureInfo info;
    info.data = stbi_load_from_memory((stbi_uc *)dataIn.data(), dataIn.size(),
                                      (int *)&info.width, (int *)&info.height,
                                      (int *)&info.channel, 0);
    info.size = info.width * info.height * info.channel;
    textureInfos.push_back(info);
    if (info.channel == 3)
      throw std::runtime_error("Texture with 3 channels not supported!");
  }
  return apiLayer->pushTexture(textureInfos.size(), textureInfos.data());
}

uint32_t
GameGraphicsModule::loadTextures(const std::vector<std::string> &names) {
  std::vector<std::vector<char>> data;
  data.reserve(names.size());
  for (const auto &name : names) {
    data.push_back(util::wholeFile(name));
  }
  return loadTextures(data);
}

} // namespace tge::graphics