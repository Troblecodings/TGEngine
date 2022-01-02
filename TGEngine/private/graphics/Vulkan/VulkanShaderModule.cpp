#include "../../../public/graphics/vulkan/VulkanShaderModule.hpp"
#include "../../../public/Error.hpp"
#include "../../../public/Util.hpp"
#include "../../../public/graphics/vulkan/VulkanModuleDef.hpp"
#include "../../../public/graphics/vulkan/VulkanShaderPipe.hpp"
#define ENABLE_OPT 1
#include <format>
#include <iostream>
#include <vulkan/vulkan.hpp>
#undef ERROR
#include <ShaderPermute.hpp>

namespace tge::shader {

using namespace vk;

#define NO_BINDING_GIVEN 65535
#define NO_LAYOUT_GIVEN 4095

inline Format getFormatFromElf(const glslang::TType &format) {
  if (format.isVector() &&
      format.getBasicType() == glslang::TBasicType::EbtFloat) {
    switch (format.getVectorSize()) {
    case 2:
      return Format::eR32G32Sfloat;
    case 3:
      return Format::eR32G32B32Sfloat;
    case 4:
      return Format::eR32G32B32A32Sfloat;
    }
  }
  throw std::runtime_error(std::string("Couldn't find Format for TType" +
                                       format.getCompleteString()));
}

inline uint32_t getSizeFromFormat(const Format format) {
  switch (format) {
  case Format::eR32G32Sfloat:
    return 8;
  case Format::eR32G32B32Sfloat:
    return 12;
  case Format::eR32G32B32A32Sfloat:
    return 16;
  default:
    throw std::runtime_error(std::string("Couldn't find size for Format ") +
                             to_string(format));
  }
}

struct VertexShaderAnalizer : public glslang::TIntermTraverser {

  VulkanShaderPipe *shaderPipe;

  VertexShaderAnalizer(VulkanShaderPipe *pipe)
      : glslang::TIntermTraverser(false, true, false), shaderPipe(pipe) {
    ioVars.reserve(10);
  }
  std::unordered_set<size_t> ioVars;

  void visitSymbol(glslang::TIntermSymbol *symbol) {
    const auto &qualifier = symbol->getQualifier();
    if (ioVars.contains(symbol->getId()))
      return;
    ioVars.emplace(symbol->getId());
    if (qualifier.layoutLocation < NO_LAYOUT_GIVEN) {
      if (qualifier.storage == glslang::TStorageQualifier::EvqVaryingIn) {
        const auto bind = qualifier.layoutBinding == NO_BINDING_GIVEN
                              ? 0
                              : qualifier.layoutBinding;
        shaderPipe->vertexInputAttributes.push_back(
            VertexInputAttributeDescription(
                qualifier.layoutLocation, bind,
                getFormatFromElf(symbol->getType())));
      }
    }
  }

  void post() {
    const auto beginItr = shaderPipe->vertexInputAttributes.begin();
    const auto endItr = shaderPipe->vertexInputAttributes.end();

    for (auto &vert : shaderPipe->vertexInputAttributes) {
      for (auto itr = beginItr; itr != endItr; itr++) {
        if (itr->location < vert.location && itr->binding == vert.binding)
          vert.offset += getSizeFromFormat(itr->format);
      }

      auto beginItrBinding = shaderPipe->vertexInputBindings.begin();
      auto endItrBinding = shaderPipe->vertexInputBindings.end();
      auto fitr = std::find_if(
          beginItrBinding, endItrBinding,
          [bind = vert.binding](auto c) { return c.binding == bind; });
      if (fitr == endItrBinding) {
        const auto index = shaderPipe->vertexInputBindings.size();
        shaderPipe->vertexInputBindings.push_back(
            VertexInputBindingDescription(vert.binding, 0));
        fitr = shaderPipe->vertexInputBindings.begin() + index;
      }
      fitr->stride += getSizeFromFormat(vert.format);
    }

    shaderPipe->inputStateCreateInfo = PipelineVertexInputStateCreateInfo(
        {}, shaderPipe->vertexInputBindings.size(),
        shaderPipe->vertexInputBindings.data(),
        shaderPipe->vertexInputAttributes.size(),
        shaderPipe->vertexInputAttributes.data());
  }
};

inline std::pair<DescriptorType, uint32_t>
getDescTypeFromELF(const glslang::TType &type) {
  const auto count = type.isArray() ? type.getOuterArraySize() : 1;
  if (type.getBasicType() == glslang::TBasicType::EbtSampler) {
    const auto sampler = type.getSampler();
    if (sampler.isImageClass() && sampler.isSubpass())
      return std::pair(DescriptorType::eInputAttachment, count);
    if (sampler.isPureSampler() || sampler.isCombined())
      return std::pair(DescriptorType::eSampler, count);
    return std::pair(DescriptorType::eSampledImage, count);
  } else if (type.getBasicType() == glslang::TBasicType::EbtBlock) {
    return std::pair(DescriptorType::eUniformBuffer, count);
  }
  std::cout << type.getQualifier().layoutAttachment << " <- Attachment";
  throw std::runtime_error("Descriptor could not be found for: " +
                           std::string(type.getCompleteString()));
}

struct GeneralShaderAnalizer : public glslang::TIntermTraverser {

  VulkanShaderPipe *shaderPipe;
  const ShaderStageFlags flags;
  std::unordered_set<uint32_t> uset;

  GeneralShaderAnalizer(VulkanShaderPipe *pipe, ShaderStageFlags flags)
      : glslang::TIntermTraverser(false, true, false), shaderPipe(pipe),
        flags(flags) {
    shaderPipe->descriptorLayoutBindings.reserve(10);
    uset.reserve(10);
  }

  void visitSymbol(glslang::TIntermSymbol *symbol) {
    const auto &type = symbol->getType();
    std::cout << type.getCompleteString() << std::endl;
    const auto &quali = type.getQualifier();
    if (quali.layoutBinding < NO_BINDING_GIVEN) {
      if (uset.contains(quali.layoutBinding))
        return;
      uset.insert(quali.layoutBinding);
      const auto desc = getDescTypeFromELF(type);
      const DescriptorSetLayoutBinding descBinding(
          quali.layoutBinding, desc.first, desc.second, flags);
      shaderPipe->descriptorLayoutBindings.push_back(descBinding);
    }
  }
};

struct ShaderInfo {
  EShLanguage language;
  std::vector<char> code;
  std::vector<std::string> additionalCode;
};

ShaderPipe VulkanShaderModule::loadShaderPipeAndCompile(
    const std::vector<std::string> &shadernames) {

}

ShaderPipe
VulkanShaderModule::createShaderPipe(const ShaderCreateInfo *shaderCreateInfo,
                                     const size_t shaderCount) {
}

void VulkanShaderModule::updateAllTextures() {
  std::vector<BindingInfo> bInfo;
  graphics::VulkanGraphicsModule *vgm =
      (graphics::VulkanGraphicsModule *)this->vgm;
  for (auto nID : textureDesc) {
    for (size_t i = 0; i < 5; i++) {
      bInfo.push_back({1, nID, BindingType::Texture, {5, UINT64_MAX}, i});
    }
    for (size_t i = 5; i < vgm->textureImages.size(); i++) {
      bInfo.push_back({1, nID, BindingType::Texture, {i, UINT64_MAX}, i});
    }
    for (size_t i = vgm->textureImages.size(); i < 255; i++) {
      bInfo.push_back({1, nID, BindingType::Texture, {5, UINT64_MAX}, i});
    }
  }
  this->bindData(bInfo.data(), bInfo.size());
}

size_t VulkanShaderModule::createBindings(ShaderPipe pipe, const size_t count) {
  VulkanShaderPipe *shaderPipe = (VulkanShaderPipe *)pipe;
  const auto layout = shaderPipe->layoutID;
  if (layout == UINT64_MAX)
    return UINT64_MAX;
  std::vector<DescriptorSetLayout> layouts(count);
  std::fill(layouts.begin(), layouts.end(), this->setLayouts[layout]);
  const DescriptorSetAllocateInfo allocInfo(this->descPools[layout], count,
                                            layouts.data());
  graphics::VulkanGraphicsModule *vgm =
      (graphics::VulkanGraphicsModule *)this->vgm;
  const auto sets = vgm->device.allocateDescriptorSets(allocInfo);
  const auto nextID = this->descSets.size();
  this->descSets.resize(nextID + count);
  std::copy(sets.begin(), sets.end(), this->descSets.begin() + nextID);

  for (size_t i = 0; i < count; i++) {
    pipeInfos.push_back({nextID + i, layout});
  }

  std::vector<BindingInfo> bInfo;
  if (defaultbindings.size() > layout) {
    auto &bindings = defaultbindings[layout];
    if (!bindings.empty()) {
      for (size_t i = 0; i < count; i++) {
        const auto nID = nextID + i;
        for (auto &b : bindings) {
          b.bindingSet = nID;
          bInfo.push_back(b);
        }
      }
      for (size_t i = 0; i < count; i++) {
        const auto nID = nextID + i;
        textureDesc.push_back(nID);
        for (size_t i = 0; i < 5; i++) {
          bInfo.push_back({1, nID, BindingType::Texture, {5, UINT64_MAX}, i});
        }
        for (size_t i = 5; i < vgm->textureImages.size(); i++) {
          bInfo.push_back({1, nID, BindingType::Texture, {i, UINT64_MAX}, i});
        }
        for (size_t i = vgm->textureImages.size(); i < 255; i++) {
          bInfo.push_back({1, nID, BindingType::Texture, {5, UINT64_MAX}, i});
        }
      }

      this->bindData(bInfo.data(), bInfo.size());
    }
  }
  return nextID;
}

void VulkanShaderModule::changeInputBindings(const ShaderPipe pipe,
                                             const size_t bindingID,
                                             const size_t buffer) {}

void VulkanShaderModule::bindData(const BindingInfo *info, const size_t count) {
  graphics::VulkanGraphicsModule *vgm =
      (graphics::VulkanGraphicsModule *)this->vgm;

  std::vector<WriteDescriptorSet> set;
  set.reserve(count);
  std::vector<DescriptorBufferInfo> bufferInfo;
  bufferInfo.resize(count);
  std::vector<DescriptorImageInfo> imgInfo;
  imgInfo.resize(count);
  for (size_t i = 0; i < count; i++) {
    const auto &cinfo = info[i];
    switch (cinfo.type) {
    case BindingType::UniformBuffer: {
      const auto &buffI = cinfo.data.buffer;
      bufferInfo[i] = (DescriptorBufferInfo(vgm->bufferList[buffI.dataID],
                                            buffI.offset, buffI.size));
      set.push_back(WriteDescriptorSet(
          descSets[cinfo.bindingSet], cinfo.binding, 0, 1,
          DescriptorType::eUniformBuffer, nullptr, bufferInfo.data() + i));
    } break;
    case BindingType::Texture:
    case BindingType::Sampler:
    case BindingType::InputAttachment: {
      const auto &tex = cinfo.data.texture;
      imgInfo[i] = DescriptorImageInfo(
          tex.sampler == UINT64_MAX ? vk::Sampler() : vgm->sampler[tex.sampler],
          tex.texture == UINT64_MAX ? vk::ImageView()
                                    : vgm->textureImageViews[tex.texture],
          ImageLayout::eShaderReadOnlyOptimal);
      set.push_back(WriteDescriptorSet(
          descSets[cinfo.bindingSet], cinfo.binding, cinfo.arrayID, 1,
          cinfo.type == BindingType::Texture ? DescriptorType::eSampledImage
          : (cinfo.type == BindingType::InputAttachment)
              ? DescriptorType::eInputAttachment
              : DescriptorType::eSampler,
          imgInfo.data() + i));
    } break;
    default:
      throw std::runtime_error("Can not find descriptor type in bind data!");
    }
  }
  vgm->device.updateDescriptorSets(set, {});
}

void VulkanShaderModule::addToRender(const size_t *bids, const size_t size,
                                     void *customData) {
  std::vector<DescriptorSet> sets;
  PipelineLayout pipeLayout;
  for (size_t i = 0; i < size; i++) {
    const auto bindingID = bids[i];
    if (this->pipeInfos.size() > bindingID && bindingID >= 0) {
      const auto &bInfo = pipeInfos[bindingID];
      const auto descSet = descSets[bInfo.descSet];
      pipeLayout = pipeLayouts[bInfo.pipeline];
      sets.push_back(descSet);
    }
  }
  ((CommandBuffer *)customData)
      ->bindDescriptorSets(PipelineBindPoint::eGraphics, pipeLayout, 0, sets,
                           {});
}

void VulkanShaderModule::addToMaterial(const graphics::Material *material,
                                       void *customData) {
  using namespace tge::graphics;
  const auto vkPipe = ((VulkanShaderPipe *)material->costumShaderData);
  const auto layOut = vkPipe->layoutID;
  if (layOut != UINT64_MAX) [[likely]] {
    ((GraphicsPipelineCreateInfo *)customData)->setLayout(pipeLayouts[layOut]);
  } else {
    ((GraphicsPipelineCreateInfo *)customData)->setLayout(defaultLayout);
  }

  // LEGACY
  if (material->type == MaterialType::TextureOnly) { // Legacy support
    const auto texMat = material->data.textureMaterial;
    if (layOut == UINT64_MAX)
      return;
    if (defaultbindings.size() <= layOut) {
      defaultbindings.resize(layOut + 1);
    }
    defaultbindings[layOut] = {{0,
                                UINT64_MAX,
                                BindingType::Sampler,
                                {texMat.textureIndex, texMat.samplerIndex}}};
  }
}

void VulkanShaderModule::init() {
  graphics::VulkanGraphicsModule *vgm =
      (graphics::VulkanGraphicsModule *)this->vgm;
  if (!vgm->isInitialiazed)
    throw std::runtime_error(
        "Vulkan module not initalized, Vulkan Shader Module cannot be used!");

  const DescriptorSetLayoutCreateInfo layoutCreate({}, {});
  defaultDescLayout = vgm->device.createDescriptorSetLayout(layoutCreate);

  const auto layoutCreateInfo = PipelineLayoutCreateInfo({}, defaultDescLayout);
  defaultLayout = vgm->device.createPipelineLayout(layoutCreateInfo);
}

void VulkanShaderModule::destroy() {
  graphics::VulkanGraphicsModule *vgm =
      (graphics::VulkanGraphicsModule *)this->vgm;
  vgm->device.destroyDescriptorSetLayout(defaultDescLayout);
  vgm->device.destroyPipelineLayout(defaultLayout);
  for (auto pool : descPools)
    vgm->device.destroyDescriptorPool(pool);
  for (auto dscLayout : setLayouts)
    vgm->device.destroyDescriptorSetLayout(dscLayout);
  for (auto pipeLayout : pipeLayouts)
    vgm->device.destroyPipelineLayout(pipeLayout);
}

} // namespace tge::shader
