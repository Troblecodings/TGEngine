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

inline shader::IOType inputTypeFromGLTF(int type) {
  namespace s = shader;
  switch (type) {
  case TINYGLTF_TYPE_VEC2:
    return s::IOType::VEC2;
  case TINYGLTF_TYPE_VEC3:
    return s::IOType::VEC3;
  case TINYGLTF_TYPE_VEC4:
    return s::IOType::VEC4;
  case TINYGLTF_TYPE_MAT4:
    return s::IOType::MAT4;
  case TINYGLTF_TYPE_MAT3:
    return s::IOType::MAT3;
  case TINYGLTF_TYPE_SCALAR:
    return s::IOType::FLOAT;
  default:
    throw std::runtime_error("Type not found in GLTF translation!");
  }
}

inline size_t loadMaterials(const Model &model, APILayer *apiLayer,
                            const size_t sampler, const size_t texture,
                            std::vector<shader::ShaderPipe> &created) {
  std::vector<Material> materials;
  materials.reserve(model.materials.size());
  for (const auto &mat : model.materials) {
    const auto &pbr = mat.pbrMetallicRoughness;
    const auto &diffuseTexture = pbr.baseColorTexture;

    Material nmMat;
    if (diffuseTexture.index != -1) [[likely]] {
      nmMat.type = MaterialType::TextureOnly;
      const auto nextSampler = model.textures[diffuseTexture.index].sampler;
      nmMat.data.textureMaterial.samplerIndex =
          nextSampler < 0 ? sampler : (nextSampler + sampler);
      nmMat.data.textureMaterial.textureIndex = diffuseTexture.index + texture;
    } else {
      nmMat.type = MaterialType::None;
    }
    nmMat.doubleSided = mat.doubleSided;
    materials.push_back(nmMat);
  }

  namespace s = shader;
  for (const auto &mesh : model.meshes) {
    const auto &prim = mesh.primitives[0];
    s::ShaderCreateInfo createInfo[2];
    auto &mat = materials[prim.material];
    for (const auto &[name, id] : prim.attributes) {
      const auto &acc = model.accessors[id];
      createInfo[0].inputs.push_back({name, inputTypeFromGLTF(acc.type), 0});
    }
    std::sort(createInfo[0].inputs.begin(), createInfo[0].inputs.end(),
              [](auto x, auto y) { return x.iotype > y.iotype; });
    for (size_t i = 0; i < createInfo[0].inputs.size(); i++) {
      auto &in = createInfo[0].inputs[i];
      in.binding = i;
      in.buffer = i;
    }
    createInfo[0].shaderType = s::ShaderType::VERTEX;
    createInfo[1].shaderType = s::ShaderType::FRAGMENT;

    createInfo[0].unifromIO.push_back({"model", s::IOType::MAT4, 2});
    createInfo[0].unifromIO.push_back({"vp", s::IOType::MAT4, 3});
    createInfo[0].instructions = {
        {{"POSITION", "1"},
         s::IOType::VEC4,
         s::InstructionType::VEC4CTR,
         "_tmpVEC"},
        {{"ublock_0.model", "_tmpVEC"},
         s::IOType::VEC4,
         s::InstructionType::MULTIPLY,
         "_tmp1"},
        {{"ublock_1.vp", "_tmp1"},
         s::IOType::VEC4,
         s::InstructionType::MULTIPLY,
         "_tmp2"},
        {{"_tmp2"}, s::IOType::VEC4, s::InstructionType::SET, "gl_Position"},
        {{"_tmp1.xyz"}, s::IOType::VEC4, s::InstructionType::SET, "POSOUT"}};

    uint32_t nextID = 0;

    createInfo[1].outputs = {{"COLOR", s::IOType::VEC4, 0},
                             {"NORMAL", s::IOType::VEC4, 1},
                             {"ROUGHNESSMETALLIC", s::IOType::VEC2, 2},
                             {"POSOUT", s::IOType::VEC4, 3}};

    createInfo[0].outputs.push_back({"POSOUT", s::IOType::VEC3, nextID});
    createInfo[1].inputs.push_back({"POSIN", s::IOType::VEC3, nextID});
    nextID++;

    if (mat.type == MaterialType::TextureOnly) {
      createInfo[0].outputs.push_back({"UV", s::IOType::VEC2, nextID});
      createInfo[0].instructions.push_back(
          {{"TEXCOORD_0"}, s::IOType::VEC4, s::InstructionType::SET, "UV"});

      createInfo[1].samplerIO.push_back({"SAMP", s::SamplerIOType::SAMPLER, 0});
      createInfo[1].samplerIO.push_back({"TEX", s::SamplerIOType::TEXTURE, 1});
      createInfo[1].inputs.push_back({"UV", s::IOType::VEC2, nextID});
      createInfo[1].instructions = {
          {{"sampler2D(TEX, SAMP)", "UV"},
           s::IOType::VEC4,
           s::InstructionType::TEXTURE,
           "C1"},
          {{"C1"}, s::IOType::VEC4, s::InstructionType::SET, "COLOR"},
          {{"vec4(POSIN, 1)"},
           s::IOType::VEC4,
           s::InstructionType::SET,
           "POSOUT"}};
      nextID++;
    } else {
      createInfo[1].instructions = {{{"vec4(1, 0, 0, 1)"},
                                     s::IOType::VEC4,
                                     s::InstructionType::SET,
                                     "COLOR"},
                                    {{"vec4(POSIN, 1)"},
                                     s::IOType::VEC4,
                                     s::InstructionType::SET,
                                     "POSOUT"}};
    };

    if (prim.attributes.contains("NORMAL")) {
      createInfo[0].outputs.push_back({"NORMALOUT", s::IOType::VEC3, nextID});
      createInfo[1].inputs.push_back({"NORMALIN", s::IOType::VEC3, nextID});
      createInfo[0].instructions.push_back(
          {{"NORMAL"}, s::IOType::VEC3, s::InstructionType::SET, "NORMALOUT"});
      createInfo[1].instructions.push_back({{"NORMALIN", "1"},
                                            s::IOType::VEC4,
                                            s::InstructionType::VEC4CTR,
                                            "_temp"});
      nextID++;
    } else {
      createInfo[1].instructions.push_back({{"1", "1", "1", "1"},
                                            s::IOType::VEC4,
                                            s::InstructionType::VEC4CTR,
                                            "_temp"});
    }
    createInfo[1].instructions.push_back(
        {{"_temp"}, s::IOType::VEC3, s::InstructionType::SET, "NORMAL"});

    createInfo[1].__code = "ROUGHNESSMETALLIC = vec2(0, 0);";

    mat.costumShaderData =
        apiLayer->getShaderAPI()->createShaderPipe(createInfo, 2);
    created.push_back(mat.costumShaderData);
  }

  return apiLayer->pushMaterials(materials.size(), materials.data());
}

inline size_t loadDataBuffers(const Model &model, APILayer *apiLayer) {
  std::vector<uint8_t *> ptr;
  ptr.reserve(model.buffers.size());
  std::vector<size_t> sizes;
  sizes.reserve(ptr.capacity());
  for (const auto &buffer : model.buffers) {
    const auto ptrto = (uint8_t *)buffer.data.data();
    ptr.push_back(ptrto);
    sizes.push_back(buffer.data.size());
  }
  return apiLayer->pushData(ptr.size(), (const uint8_t **)ptr.data(),
                            sizes.data(), DataType::VertexIndexData);
}

inline void pushRender(const Model &model, APILayer *apiLayer,
                       const size_t dataId, const size_t materialId,
                       const size_t nodeID,
                       const std::vector<size_t> bindings) {
  std::vector<RenderInfo> renderInfos;
  renderInfos.reserve(1000);
  for (size_t i = 0; i < model.meshes.size(); i++) {
    const auto &mesh = model.meshes[i];
    const auto bItr = model.nodes.begin();
    const auto eItr = model.nodes.end();
    const auto oItr = std::find_if(
        bItr, eItr, [idx = i](const Node &node) { return node.mesh == idx; });
    const auto nID =
        oItr != eItr ? std::distance(bItr, oItr) + nodeID : UINT64_MAX;
    const auto bID = bindings[nID];
    for (const auto &prim : mesh.primitives) {
      std::vector<std::tuple<int, int, int>> strides;
      strides.reserve(prim.attributes.size());

      for (const auto &attr : prim.attributes) {
        const auto &vertAccesor = model.accessors[attr.second];
        const auto &vertView = model.bufferViews[vertAccesor.bufferView];
        const auto bufferID = vertView.buffer + dataId;
        const auto vertOffset = vertView.byteOffset + vertAccesor.byteOffset;
        strides.push_back(
            std::make_tuple(vertAccesor.type, bufferID, vertOffset));
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
            bufferOffsets,
            bID};
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
            bufferOffsets,
            bID};
        renderInfos.push_back(renderInfo);
      }
    }
  }

  apiLayer->pushRender(renderInfos.size(), renderInfos.data());
}

inline size_t loadNodes(const Model &model, APILayer *apiLayer,
                        const size_t nextNodeID, GameGraphicsModule *ggm,
                        const std::vector<shader::ShaderPipe> &created) {
  std::vector<NodeInfo> nodeInfos = {};
  const auto amount = model.nodes.size();
  nodeInfos.resize(amount + 1);
  if (amount != 0) [[likely]] {
    for (size_t i = 0; i < amount; i++) {
      const auto &node = model.nodes[i];
      const auto infoID = i + 1;
      auto &info = nodeInfos[infoID];
      if (!node.translation.empty()) {
        info.transforms.translation.x = (float)node.translation[0];
        info.transforms.translation.y = (float)node.translation[1];
        info.transforms.translation.z = (float)node.translation[2];
      }
      if (!node.scale.empty()) {
        info.transforms.scale.x = (float)node.scale[0];
        info.transforms.scale.y = (float)node.scale[1];
        info.transforms.scale.z = (float)node.scale[2];
      }
      if (!node.rotation.empty()) {
        info.transforms.rotation =
            glm::quat((float)node.rotation[3], (float)node.rotation[0],
                      (float)node.rotation[1], (float)node.rotation[2]);
      }
      for (const auto id : node.children) {
        nodeInfos[id + 1].parent = nextNodeID + infoID;
      }
      if (node.mesh >= 0 && created.size() > node.mesh) [[likely]] {
        info.bindingID =
            apiLayer->getShaderAPI()->createBindings(created[node.mesh]);
      } else {
        info.bindingID =
            apiLayer->getShaderAPI()->createBindings(ggm->defaultPipe);
      }
    }
    for (auto &nInfo : nodeInfos) {
      if (nInfo.parent == UINT64_MAX) {
        nInfo.parent = nextNodeID;
      }
    }
  } else {
    const auto startID =
        apiLayer->getShaderAPI()->createBindings(ggm->defaultPipe);
    nodeInfos[0].bindingID = startID;
  }
  return ggm->addNode(nodeInfos.data(), nodeInfos.size());
}

GameGraphicsModule::GameGraphicsModule(APILayer *apiLayer,
                                       WindowModule *winModule) {
  const auto prop = winModule->getWindowProperties();
  this->apiLayer = apiLayer;
  this->windowModule = winModule;
  // TODO Cleanup
  this->projectionMatrix =
      glm::perspective(glm::radians(45.0f),
                       (float)prop.width / (float)prop.height, 0.1f, 100.0f);
  this->projectionMatrix[1][1] *= -1;
  this->viewMatrix = glm::lookAt(glm::vec3(0, 0.5f, 1), glm::vec3(0, 0, 0),
                                 glm::vec3(0, 1, 0));
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

  std::vector<shader::ShaderPipe> createdShader;
  const auto materials =
      model.materials.empty()
          ? defaultMaterial
          : loadMaterials(model, apiLayer, samplerId, textureId, createdShader);

  const auto nId = loadNodes(model, apiLayer, node.size(), this, createdShader);

  pushRender(model, apiLayer, dataId, materials, nId + 1, this->bindingID);

  return nId;
}

main::Error GameGraphicsModule::init() {
  const auto size = this->node.size();
  glm::mat4 projView = this->projectionMatrix * this->viewMatrix;
  modelMatrices.reserve(UINT16_MAX);
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
  }

  std::array mvpsPtr = {(const uint8_t *)modelMatrices.data(),
                        (const uint8_t *)&projView};
  std::array arrSize = {UINT16_MAX * sizeof(glm::mat4), sizeof(glm::mat4)};
  dataID = apiLayer->pushData(mvpsPtr.size(), mvpsPtr.data(), arrSize.data(),
                              DataType::Uniform);
  const Material defMat(defaultPipe = apiLayer->loadShader(MaterialType::None));
  defaultMaterial = apiLayer->pushMaterials(1, &defMat);
  return main::Error::NONE;
}

void GameGraphicsModule::tick(double time) {
  const auto size = this->node.size();
  const auto projView = this->projectionMatrix * this->viewMatrix;
  apiLayer->changeData(dataID + 1, (const uint8_t *)&projView,
                       sizeof(glm::mat4));
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
      apiLayer->changeData(dataID, (const uint8_t *)&modelMatrices[i],
                           sizeof(glm::mat4), i * sizeof(glm::mat4));
    }
  }
  std::fill(begin(this->status), end(this->status), 0);
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
  std::vector<shader::BindingInfo> bindings;
  bindings.reserve(count);
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
    if (nodeI.bindingID != UINT64_MAX) [[likely]] {
      const auto mvp = modelMatrices[nodeID];
      const auto off = sizeof(mvp) * (nodeID + i);
      apiLayer->changeData(dataID, (const uint8_t *)&mvp, sizeof(mvp), off);
      bindings.push_back({2,
                          nodeI.bindingID,
                          shader::BindingType::UniformBuffer,
                          {dataID, sizeof(mvp), off}});
      bindings.push_back({3,
                          nodeI.bindingID,
                          shader::BindingType::UniformBuffer,
                          {dataID + 1, sizeof(mvp), 0}});
    }
    bindingID.push_back(nodeI.bindingID);
  }
  apiLayer->getShaderAPI()->bindData(bindings.data(), bindings.size());
  return nodeID;
}

void GameGraphicsModule::updateTransform(const size_t nodeID,
                                         const NodeTransform &transform) {
  this->node[nodeID] = transform;
  this->status[nodeID] = 1;
}

} // namespace tge::graphics