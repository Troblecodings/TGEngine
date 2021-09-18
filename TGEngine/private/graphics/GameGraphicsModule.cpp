#include "../../public/graphics/GameGraphicsModule.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../public/Util.hpp"
#include "../../public/graphics/GameShaderModule.hpp"
#include "../../public/graphics/vulkan/VulkanShaderPipe.hpp"
#include "../../public/headerlibs/tiny_gltf.h"
#include <array>
#include <glm/gtx/transform.hpp>
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
    return vk::Format::eR32Sfloat;
  case 8:
    return vk::Format::eR32G32Sfloat;
  case 12:
    return vk::Format::eR32G32B32Sfloat;
  case 16:
    return vk::Format::eR32G32B32A32Sfloat;
  default:
    throw std::runtime_error("Couldn't find format");
  }
}

inline size_t loadSampler(const Model &model, APILayer *apiLayer) {
  size_t samplerIndex = -1;
  for (const auto &smplr : model.samplers) {
    const SamplerInfo samplerInfo = {
        gltfToAPI(smplr.minFilter, FilterSetting::LINEAR),
        gltfToAPI(smplr.minFilter, FilterSetting::LINEAR),
        gltfToAPI(smplr.wrapS, AddressMode::REPEAT),
        gltfToAPI(smplr.wrapT, AddressMode::REPEAT)};
    samplerIndex = apiLayer->pushSampler(samplerInfo);
  }
  samplerIndex -= model.samplers.size() - 1;

  if (!model.images.empty()) {
    if (model.samplers.empty()) { // default sampler
      const SamplerInfo samplerInfo = {
          FilterSetting::LINEAR, FilterSetting::LINEAR, AddressMode::REPEAT,
          AddressMode::REPEAT};
      samplerIndex = apiLayer->pushSampler(samplerInfo);
    }
  }
  return samplerIndex;
}

inline size_t loadTexturesFM(const Model &model, APILayer *apiLayer) {
  std::vector<TextureInfo> textureInfos;
  textureInfos.reserve(model.images.size());
  for (const auto &img : model.images) {
    if (!img.image.empty()) [[likely]] {
      const TextureInfo info{(uint8_t *)img.image.data(),
                             (uint32_t)img.image.size(), (uint32_t)img.width,
                             (uint32_t)img.height, (uint32_t)img.component};
      textureInfos.push_back(info);
    } else {
      throw std::runtime_error("Not implemented!");
    }
  }
  if (!textureInfos.empty())
    return apiLayer->pushTexture(textureInfos.size(), textureInfos.data());
  return -1;
}

inline size_t loadMaterials(const Model &model, APILayer *apiLayer,
                            void *shaderPipe, const size_t sampler,
                            const size_t texture) {
  std::vector<Material> materials;
  materials.reserve(model.materials.size());
  for (const auto &mat : model.materials) {
    const auto &pbr = mat.pbrMetallicRoughness;
    const auto &diffuseTexture = pbr.baseColorTexture;

    Material nmMat(shaderPipe);
    nmMat.type = MaterialType::TextureOnly;
    const auto nextSampler = model.textures[diffuseTexture.index].sampler;
    nmMat.data.textureMaterial.samplerIndex =
        nextSampler < 0 ? sampler : (nextSampler + sampler);
    nmMat.data.textureMaterial.textureIndex = diffuseTexture.index + texture;
    materials.push_back(nmMat);
  }

#pragma region This is speculative
  auto pipe = (tge::shader::VulkanShaderPipe *)(shaderPipe == nullptr
                                                    ? apiLayer->loadShader(
                                                          MaterialType::None)
                                                    : shaderPipe);
  pipe->vertexInputAttributes.clear();
  pipe->vertexInputBindings.clear();

  for (const auto &mesh : model.meshes) {
    for (const auto &prim : mesh.primitives) {
      std::vector<int> strides;
      strides.reserve(prim.attributes.size());

      for (const auto &attr : prim.attributes) {
        const auto &vertAccesor = model.accessors[attr.second];
        const auto &vertView = model.bufferViews[vertAccesor.bufferView];
        strides.push_back(vertAccesor.ByteStride(vertView));
      }
      std::sort(strides.rbegin(), strides.rend());

      uint32_t location = 0;
      for (const auto &stride : strides) {
        pipe->vertexInputAttributes.push_back(
            vk::VertexInputAttributeDescription(location, location,
                                                getFormatFromStride(stride)));
        pipe->vertexInputBindings.push_back(
            vk::VertexInputBindingDescription(location, stride));
        location++;
      }
      pipe->inputStateCreateInfo = vk::PipelineVertexInputStateCreateInfo(
          {}, pipe->vertexInputBindings, pipe->vertexInputAttributes);
    }
  }
#pragma endregion

  if (materials.empty()) {
    const Material defMat(pipe);
    return apiLayer->pushMaterials(1, &defMat);
  } else {
    return apiLayer->pushMaterials(materials.size(), materials.data());
  }
}

inline size_t loadDataBuffers(const Model &model, APILayer *apiLayer) {
  std::vector<uint8_t *> ptr;
  ptr.reserve(model.buffers.size());
  std::vector<size_t> sizes;
  sizes.reserve(ptr.capacity());
  for (const auto &mesh : model.meshes) {
    for (const auto &prim : mesh.primitives) {
      if (prim.indices >= 0) [[likely]] {
        const auto &indexAccesor = model.accessors[prim.indices];
        const auto &indexView = model.bufferViews[indexAccesor.bufferView];
        const auto &indexBuffer = model.buffers[indexView.buffer];
        ptr.push_back((uint8_t *)indexBuffer.data.data());
        sizes.push_back(indexBuffer.data.size());
      }

      for (const auto &attr : prim.attributes) {
        const auto &vertAccesor = model.accessors[attr.second];
        const auto &vertView = model.bufferViews[vertAccesor.bufferView];
        const auto &vertBuffer = model.buffers[vertView.buffer];

        const auto ptrto = (uint8_t *)vertBuffer.data.data();
        if (std::find(ptr.begin(), ptr.end(), ptrto) != ptr.end())
          continue;
        ptr.push_back(ptrto);
        sizes.push_back(vertBuffer.data.size());
      }
    }
  }
  return apiLayer->pushData(ptr.size(), (const uint8_t **)ptr.data(),
                            sizes.data(), DataType::VertexIndexData);
}

inline void pushRender(const Model &model, APILayer *apiLayer,
                       const size_t dataId, const size_t materialId) {
  std::vector<RenderInfo> renderInfos;
  renderInfos.reserve(1000);
  for (const auto &mesh : model.meshes) {
    for (const auto &prim : mesh.primitives) {
      std::vector<std::tuple<int, int, int>> strides;
      strides.reserve(prim.attributes.size());

      for (const auto &attr : prim.attributes) {
        const auto &vertAccesor = model.accessors[attr.second];
        const auto &vertView = model.bufferViews[vertAccesor.bufferView];
        const auto bufferID = vertView.buffer + dataId;
        const auto vertOffset = vertView.byteOffset + vertAccesor.byteOffset;
        strides.push_back(std::make_tuple(vertAccesor.ByteStride(vertView),
                                          bufferID, vertOffset));
      }

      std::sort(strides.rbegin(), strides.rend(),
                [](auto x, auto y) { return std::get<0>(x) < std::get<0>(y); });
      std::vector<size_t> bufferIndicies;
      bufferIndicies.reserve(strides.size());
      std::vector<size_t> bufferOffsets;
      bufferOffsets.reserve(bufferIndicies.capacity());
      for (auto &stride : strides) {
        bufferIndicies.push_back(std::get<1>(stride));
        bufferOffsets.push_back(std::get<2>(stride));
      }

      if (prim.indices >= 0) [[likely]] {
        const auto &indexAccesor = model.accessors[prim.indices];
        const auto &indexView = model.bufferViews[indexAccesor.bufferView];
        const auto indexOffset = indexView.byteOffset + indexAccesor.byteOffset;
        const IndexSize indextype =
            indexView.byteStride == 4 ? IndexSize::UINT32 : IndexSize::UINT16;
        const RenderInfo renderInfo = {
            bufferIndicies,
            indexView.buffer + dataId,
            prim.material == -1 ? materialId : prim.material + materialId,
            indexAccesor.count,
            1,
            indexOffset,
            indextype,
            bufferOffsets};
        renderInfos.push_back(renderInfo);
      } else {
        const auto accessorID = prim.attributes.begin()->second;
        const auto &vertAccesor = model.accessors[accessorID];
        const RenderInfo renderInfo = {
            bufferIndicies,
            0,
            prim.material == -1 ? materialId : prim.material + materialId,
            0,
            1,
            vertAccesor.count,
            IndexSize::NONE,
            bufferOffsets};
        renderInfos.push_back(renderInfo);
      }
    }
  }

  apiLayer->pushRender(renderInfos.size(), renderInfos.data());
}

GameGraphicsModule::GameGraphicsModule(APILayer *apiLayer,
                                       WindowModule *winModule) {
  const auto prop = winModule->getWindowProperties();
  this->apiLayer = apiLayer;
  this->windowModule = winModule;
  // TODO Cleanup
  this->projectionMatrix =
      glm::perspective(glm::radians(45.0f),
                       (float)prop.width / (float)prop.height, 0.01f, 100.0f);
  this->projectionMatrix[1][1] *= -1;
  this->viewMatrix =
      glm::lookAt(glm::vec3(0, -0.5f, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

size_t GameGraphicsModule::loadModel(const std::vector<char> &data,
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
    return UINT64_MAX;
  }

  if (!warning.empty()) {
    printf("[GLTF][WARN]: %s\n", warning.c_str());
  }

  const auto samplerId = loadSampler(model, apiLayer);

  const auto textureId = loadTexturesFM(model, apiLayer);

  const auto dataId = loadDataBuffers(model, apiLayer);

  const auto materials =
      loadMaterials(model, apiLayer, shaderPipe, samplerId, textureId);

    // TODO PUT INTO
  const NodeInfo nodeInfo = {materials};
  const auto nId = addNode(&nodeInfo, 1);

  pushRender(model, apiLayer, dataId, materials);

  return nId;
}

main::Error GameGraphicsModule::init() {
  const auto size = this->node.size();
  std::vector<glm::mat4> mvps;
  mvps.resize(100000);
  glm::mat4 projView = this->projectionMatrix * this->viewMatrix;
  for (size_t i = 0; i < size; i++) {
    const auto &transform = this->node[i];
    const auto parantID = this->parents[i];
    const auto mMatrix = glm::translate(transform.translation) *
                         glm::scale(transform.scale) *
                         glm::toMat4(transform.rotation);
    if (parantID < size) {
      modelMatrices[i] = modelMatrices[parantID] * mMatrix;
    } else {
      modelMatrices[i] = mMatrix;
    }
    mvps[i] = projView * modelMatrices[i];
  }
  const uint8_t *mvpsPtr = (uint8_t *)mvps.data();
  if (dataID != UINT64_MAX) {
    apiLayer->changeData(dataID, mvpsPtr, mvps.size() * sizeof(glm::mat4));
  } else {
    const auto arrSize = mvps.size() * sizeof(glm::mat4);
    dataID = apiLayer->pushData(1, &mvpsPtr, &arrSize, DataType::Uniform);
  }
  allDirty = false;
  return main::Error::NONE;
}

void GameGraphicsModule::tick(double time) {
  if (!allDirty) {
    const auto size = this->node.size();
    const auto projView = this->projectionMatrix * this->viewMatrix;
    for (size_t i = 0; i < size; i++) {
      const auto parantID = this->parents[i];
      if (this->status[i] == 1 || (parantID < size && this->status[parantID])) {
        const auto &transform = this->node[i];
        const auto mMatrix = glm::translate(transform.translation) *
                             glm::scale(transform.scale) *
                             glm::toMat4(transform.rotation);
        if (parantID < size) {
          modelMatrices[i] = modelMatrices[parantID] * mMatrix;
        } else {
          modelMatrices[i] = mMatrix;
        }
        const auto mvp = projView * modelMatrices[i];
        apiLayer->changeData(dataID, (const uint8_t *)&mvp,
                             sizeof(glm::mat4), i * sizeof(glm::mat4));
      }
    }
    std::fill(status.begin(), status.end(), 0);
    return;
  }
  init();
}

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

size_t GameGraphicsModule::addNode(const NodeInfo *nodeInfos,
                                   const size_t count) {
  const auto nodeID = node.size();
  node.reserve(nodeID + count);
  for (size_t i = 0; i < count; i++) {
    const auto nodeI = nodeInfos[i];
    const auto nodeIndex = (nodeID + i);
    node.push_back(nodeI.transforms);

    const auto mMatrix = glm::translate(nodeI.transforms.translation) *
                         glm::scale(nodeI.transforms.scale) *
                         glm::toMat4(nodeI.transforms.rotation);
    if (nodeI.parent < nodeIndex) {
      modelMatrices.push_back(modelMatrices[nodeI.parent] * mMatrix);
      parents.push_back(nodeI.parent);
    } else {
      modelMatrices.push_back(mMatrix);
      parents.push_back(UINT64_MAX);
    }
    status.push_back(0);
    if (nodeI.material != UINT64_MAX) {
      const auto mvp = projectionMatrix * viewMatrix * modelMatrices[nodeID];
      apiLayer->changeData(dataID, (const uint8_t *)&mvp, sizeof(mvp),
                           sizeof(mvp) * nodeID);
      //const shader::BindingInfo info = {2,           nodeI.material,
      //                          dataID,      BindingType::UniformBuffer,
      //                          sizeof(mvp), sizeof(mvp) * nodeID};
      //apiLayer->bindData(info);
    }
  }
  return nodeID;
}

void GameGraphicsModule::updateTransform(const size_t nodeID,
                                         const NodeTransform &transform) {
  this->node[nodeID] = transform;
  this->status[nodeID] = 1;
}

} // namespace tge::graphics