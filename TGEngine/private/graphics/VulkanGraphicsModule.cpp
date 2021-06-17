#include "../../public/graphics/VulkanGraphicsModule.hpp"
#include "../../public/Error.hpp"
#include "../../public/graphics/WindowModule.hpp"
#include <array>
#include <glslang/Include/intermediate.h>
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <iostream>
#include <mutex>
#ifdef WIN32
#include <Windows.h>
#define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL 1
#define VK_USE_PLATFORM_WIN32_KHR 1
#endif // WIN32
#include "../../public/Util.hpp"
#include <vector>
#define VULKAN_HPP_HAS_SPACESHIP_OPERATOR
#include <unordered_set>
#include <vulkan/vulkan.hpp>

_CONSTEXPR20_CONTAINER std::string operator""_str(const char *chr,
                                                  std::size_t size) {
  return std::string(chr, size);
}

namespace tge::graphics {

constexpr TBuiltInResource DefaultTBuiltInResource = {
    /* .MaxLights = */ 32,
    /* .MaxClipPlanes = */ 6,
    /* .MaxTextureUnits = */ 32,
    /* .MaxTextureCoords = */ 32,
    /* .MaxVertexAttribs = */ 64,
    /* .MaxVertexUniformComponents = */ 4096,
    /* .MaxVaryingFloats = */ 64,
    /* .MaxVertexTextureImageUnits = */ 32,
    /* .MaxCombinedTextureImageUnits = */ 80,
    /* .MaxTextureImageUnits = */ 32,
    /* .MaxFragmentUniformComponents = */ 4096,
    /* .MaxDrawBuffers = */ 32,
    /* .MaxVertexUniformVectors = */ 128,
    /* .MaxVaryingVectors = */ 8,
    /* .MaxFragmentUniformVectors = */ 16,
    /* .MaxVertexOutputVectors = */ 16,
    /* .MaxFragmentInputVectors = */ 15,
    /* .MinProgramTexelOffset = */ -8,
    /* .MaxProgramTexelOffset = */ 7,
    /* .MaxClipDistances = */ 8,
    /* .MaxComputeWorkGroupCountX = */ 65535,
    /* .MaxComputeWorkGroupCountY = */ 65535,
    /* .MaxComputeWorkGroupCountZ = */ 65535,
    /* .MaxComputeWorkGroupSizeX = */ 1024,
    /* .MaxComputeWorkGroupSizeY = */ 1024,
    /* .MaxComputeWorkGroupSizeZ = */ 64,
    /* .MaxComputeUniformComponents = */ 1024,
    /* .MaxComputeTextureImageUnits = */ 16,
    /* .MaxComputeImageUniforms = */ 8,
    /* .MaxComputeAtomicCounters = */ 8,
    /* .MaxComputeAtomicCounterBuffers = */ 1,
    /* .MaxVaryingComponents = */ 60,
    /* .MaxVertexOutputComponents = */ 64,
    /* .MaxGeometryInputComponents = */ 64,
    /* .MaxGeometryOutputComponents = */ 128,
    /* .MaxFragmentInputComponents = */ 128,
    /* .MaxImageUnits = */ 8,
    /* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
    /* .MaxCombinedShaderOutputResources = */ 8,
    /* .MaxImageSamples = */ 0,
    /* .MaxVertexImageUniforms = */ 0,
    /* .MaxTessControlImageUniforms = */ 0,
    /* .MaxTessEvaluationImageUniforms = */ 0,
    /* .MaxGeometryImageUniforms = */ 0,
    /* .MaxFragmentImageUniforms = */ 8,
    /* .MaxCombinedImageUniforms = */ 8,
    /* .MaxGeometryTextureImageUnits = */ 16,
    /* .MaxGeometryOutputVertices = */ 256,
    /* .MaxGeometryTotalOutputComponents = */ 1024,
    /* .MaxGeometryUniformComponents = */ 1024,
    /* .MaxGeometryVaryingComponents = */ 64,
    /* .MaxTessControlInputComponents = */ 128,
    /* .MaxTessControlOutputComponents = */ 128,
    /* .MaxTessControlTextureImageUnits = */ 16,
    /* .MaxTessControlUniformComponents = */ 1024,
    /* .MaxTessControlTotalOutputComponents = */ 4096,
    /* .MaxTessEvaluationInputComponents = */ 128,
    /* .MaxTessEvaluationOutputComponents = */ 128,
    /* .MaxTessEvaluationTextureImageUnits = */ 16,
    /* .MaxTessEvaluationUniformComponents = */ 1024,
    /* .MaxTessPatchComponents = */ 120,
    /* .MaxPatchVertices = */ 32,
    /* .MaxTessGenLevel = */ 64,
    /* .MaxViewports = */ 16,
    /* .MaxVertexAtomicCounters = */ 0,
    /* .MaxTessControlAtomicCounters = */ 0,
    /* .MaxTessEvaluationAtomicCounters = */ 0,
    /* .MaxGeometryAtomicCounters = */ 0,
    /* .MaxFragmentAtomicCounters = */ 8,
    /* .MaxCombinedAtomicCounters = */ 8,
    /* .MaxAtomicCounterBindings = */ 1,
    /* .MaxVertexAtomicCounterBuffers = */ 0,
    /* .MaxTessControlAtomicCounterBuffers = */ 0,
    /* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
    /* .MaxGeometryAtomicCounterBuffers = */ 0,
    /* .MaxFragmentAtomicCounterBuffers = */ 1,
    /* .MaxCombinedAtomicCounterBuffers = */ 1,
    /* .MaxAtomicCounterBufferSize = */ 16384,
    /* .MaxTransformFeedbackBuffers = */ 4,
    /* .MaxTransformFeedbackInterleavedComponents = */ 64,
    /* .MaxCullDistances = */ 8,
    /* .MaxCombinedClipAndCullDistances = */ 8,
    /* .MaxSamples = */ 4,
    /* .maxMeshOutputVerticesNV = */ 256,
    /* .maxMeshOutputPrimitivesNV = */ 512,
    /* .maxMeshWorkGroupSizeX_NV = */ 32,
    /* .maxMeshWorkGroupSizeY_NV = */ 1,
    /* .maxMeshWorkGroupSizeZ_NV = */ 1,
    /* .maxTaskWorkGroupSizeX_NV = */ 32,
    /* .maxTaskWorkGroupSizeY_NV = */ 1,
    /* .maxTaskWorkGroupSizeZ_NV = */ 1,
    /* .maxMeshViewCountNV = */ 4,
    /* .maxDualSourceDrawBuffersEXT = */ 1,

    /* .limits = */
    {
        /* .nonInductiveForLoops = */ 1,
        /* .whileLoops = */ 1,
        /* .doWhileLoops = */ 1,
        /* .generalUniformIndexing = */ 1,
        /* .generalAttributeMatrixVectorIndexing = */ 1,
        /* .generalVaryingIndexing = */ 1,
        /* .generalSamplerIndexing = */ 1,
        /* .generalVariableIndexing = */ 1,
        /* .generalConstantMatrixVectorIndexing = */ 1,
    }};

constexpr std::array layerToEnable = {"VK_LAYER_KHRONOS_validation",
                                      "VK_LAYER_VALVE_steam_overlay",
                                      "VK_LAYER_NV_optimus"};

constexpr std::array extensionToEnable = {VK_KHR_SURFACE_EXTENSION_NAME
#ifdef WIN32
                                          ,
                                          VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#endif // WIN32
#ifdef DEBUG
                                          ,
                                          VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif

};

using namespace vk;

Result verror = Result::eSuccess;

#define VERROR(rslt)                                                           \
  if (rslt != Result::eSuccess) {                                              \
    verror = rslt;                                                             \
    main::error = main::Error::VULKAN_ERROR;                                   \
    printf("Vulkan error %d!", (uint32_t)verror);                              \
  }

struct VulkanShaderPipe;

class VulkanGraphicsModule : public APILayer {

private:
  Instance instance;
  PhysicalDevice physicalDevice;
  Device device;
  SurfaceKHR surface;
  SurfaceFormatKHR format;
  Format depthFormat = Format::eUndefined;
  SwapchainKHR swapchain;
  std::vector<Image> images;
  RenderPass renderpass;
  std::vector<ImageView> imageviews;
  std::vector<Framebuffer> framebuffer;
  CommandPool pool;
  std::vector<CommandBuffer> cmdbuffer;
  std::vector<Pipeline> pipelines;
  Queue queue;
  uint32_t queueIndex;
  Semaphore waitSemaphore;
  Semaphore signalSemaphore;
  Fence commandBufferFence;
  std::vector<ShaderModule> shaderModules;
  uint32_t memoryTypeHostVisibleCoherent;
  uint32_t memoryTypeDeviceLocal;
  std::vector<Buffer> bufferList;
  std::vector<size_t> bufferSizeList;
  std::vector<DeviceMemory> bufferMemoryList;
  Viewport viewport;
  std::vector<CommandBuffer> secondaryCommandBuffer;
  std::mutex commandBufferRecording; // protects secondaryCommandBuffer from
                                     // memory invalidation
  Image depthImage;
  DeviceMemory depthImageMemory;
  ImageView depthImageView;
  std::vector<PipelineLayout> pipelineLayouts;
  std::vector<Sampler> sampler;
  std::vector<Image> textureImages;
  std::vector<DeviceMemory> textureMemorys;
  std::vector<ImageView> textureImageViews;
  std::vector<VulkanShaderPipe *> shaderPipes;

#ifdef DEBUG
  DebugUtilsMessengerEXT debugMessenger;
#endif

  main::Error init() override;

  void tick(double time) override;

  void destroy() override;

  size_t pushMaterials(const size_t materialcount,
                       const Material *materials) override;

  size_t pushData(const size_t dataCount, const uint8_t **data,
                  const size_t *dataSizes, const DataType type) override;

  void pushRender(const size_t renderInfoCount,
                  const RenderInfo *renderInfos) override;

  size_t pushSampler(const SamplerInfo &sampler) override;

  size_t pushTexture(const size_t textureCount,
                     const TextureInfo *textures) override;

  void *loadShader(const MaterialType type) override;
};

inline void waitForImageTransition(
    const CommandBuffer &curBuffer, const ImageLayout oldLayout,
    const ImageLayout newLayout, const uint32_t queueFamily, const Image image,
    const ImageSubresourceRange &subresource,
    const PipelineStageFlags srcFlags = PipelineStageFlagBits::eTopOfPipe,
    const AccessFlags srcAccess = AccessFlagBits::eNoneKHR,
    const PipelineStageFlags dstFlags = PipelineStageFlagBits::eAllGraphics,
    const AccessFlags dstAccess = AccessFlagBits::eNoneKHR) {
  const ImageMemoryBarrier imageMemoryBarrier(srcAccess, dstAccess, oldLayout,
                                              newLayout, queueFamily,
                                              queueFamily, image, subresource);
  curBuffer.pipelineBarrier(srcFlags, dstFlags, DependencyFlagBits::eByRegion,
                            {}, {}, imageMemoryBarrier);
}

inline EShLanguage getLang(const std::string &str) {
  if (str.compare("vert") == 0)
    return EShLanguage::EShLangVertex;
  if (str.compare("frag") == 0)
    return EShLanguage::EShLangFragment;
  throw std::runtime_error(std::string("Couldn't find EShLanguage for ") + str);
}

inline ShaderStageFlagBits getStageFromLang(const EShLanguage lang) {
  switch (lang) {
  case EShLanguage::EShLangVertex:
    return ShaderStageFlagBits::eVertex;
  case EShLanguage::EShLangFragment:
    return ShaderStageFlagBits::eFragment;
  default:
    throw std::runtime_error(
        std::string("Couldn't find ShaderStageFlagBits for EShLanguage ") +
        std::to_string(lang));
  }
}

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

struct VulkanShaderPipe {
  std::vector<std::pair<std::vector<uint32_t>, ShaderStageFlagBits>> shader;
  std::vector<PipelineShaderStageCreateInfo> pipelineShaderStage;
  std::vector<VertexInputBindingDescription> vertexInputBindings;
  std::vector<VertexInputAttributeDescription> vertexInputAttributes;
  PipelineVertexInputStateCreateInfo inputStateCreateInfo;
  PipelineRasterizationStateCreateInfo rasterization;
  std::vector<DescriptorSetLayoutCreateInfo> descriptorLayout;
  std::vector<std::vector<DescriptorSetLayoutBinding>> descriptorLayoutBindings;
  std::vector<DescriptorSet> descriptorSets;
  DescriptorPool descPool;
  PipelineLayout layout;
};

#define NO_BINDING_GIVEN 65535
#define NO_LAYOUT_GIVEN 4095

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
    if (sampler.isPureSampler() || sampler.isCombined())
      return std::pair(DescriptorType::eSampler, count);
    return std::pair(DescriptorType::eSampledImage, count);
  } else if (type.getBasicType() == glslang::TBasicType::EbtBlock) {
    return std::pair(DescriptorType::eUniformBuffer, count);
  }
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
    shaderPipe->descriptorLayoutBindings.push_back({});
    shaderPipe->descriptorLayoutBindings.back().reserve(20);
    shaderPipe->descriptorLayout.reserve(10);
    uset.reserve(10);
  }

  void visitSymbol(glslang::TIntermSymbol *symbol) {
    const auto &type = symbol->getType();
    const auto &quali = type.getQualifier();
    if (quali.layoutBinding < NO_BINDING_GIVEN) {
      if (uset.contains(quali.layoutBinding))
        return;
      uset.insert(quali.layoutBinding);
      const auto desc = getDescTypeFromELF(type);
      const DescriptorSetLayoutBinding descBinding(
          quali.layoutBinding, desc.first, desc.second, flags);
      shaderPipe->descriptorLayoutBindings.back().push_back(descBinding);
      std::cout << type.getCompleteString() << std::endl;
    }
  }

  void post() {
    const auto &vec = shaderPipe->descriptorLayoutBindings.back();
    if (!vec.empty()) {
      const DescriptorSetLayoutCreateInfo setLayoutCreateInfo({}, vec);
      shaderPipe->descriptorLayout.push_back(setLayoutCreateInfo);
    }
  }
};

inline void submitAndWait(const Device &device, const Queue &queue,
                          const CommandBuffer &cmdBuf) {
  const FenceCreateInfo fenceCreateInfo;
  const auto fence = device.createFence(fenceCreateInfo);

  const SubmitInfo submitInfo({}, {}, cmdBuf, {});
  queue.submit(submitInfo, fence);

  const Result result = device.waitForFences(fence, true, UINT64_MAX);
  VERROR(result);

  device.destroyFence(fence);
}

void __implIntermToVulkanPipe(VulkanShaderPipe *shaderPipe,
                              const glslang::TIntermediate *interm,
                              const EShLanguage langName) {
  const auto node = interm->getTreeRoot();
  if (langName == EShLangVertex) {
    VertexShaderAnalizer analizer(shaderPipe);
    node->traverse(&analizer);
    analizer.post();
  }
  const auto flags = getStageFromLang(langName);
  GeneralShaderAnalizer generalAnalizer(shaderPipe, flags);
  node->traverse(&generalAnalizer);
  generalAnalizer.post();

  shaderPipe->shader.push_back(std::pair(std::vector<uint32_t>(), flags));
  glslang::GlslangToSpv(*interm, shaderPipe->shader.back().first);
}

VulkanShaderPipe *__implLoadShaderPipeAndCompile(
    std::vector<std::pair<std::vector<uint8_t>, EShLanguage>> &vector) {
  if (vector.size() == 0)
    return nullptr;
  VulkanShaderPipe *shaderPipe = new VulkanShaderPipe();
  glslang::InitializeProcess();
  util::OnExit e1(glslang::FinalizeProcess);
  shaderPipe->shader.reserve(vector.size());

  for (auto &pair : vector) {
    const auto &data = pair.first;
    const auto langName = pair.second;
    if (data.empty()) {
      delete shaderPipe;
      return nullptr;
    }
    glslang::TShader shader(langName);
    const auto ptr = data.data();
    shader.setStrings((const char *const *)(&ptr), 1);
    shader.setEnvInput(glslang::EShSourceGlsl, langName,
                       glslang::EShClientVulkan, 100);
    shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
    shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);
    if (!shader.parse(&DefaultTBuiltInResource, 450, true,
                      EShMessages::EShMsgVulkanRules)) {
      printf("======== Shader compile error ==========\n\n%s",
             shader.getInfoLog());
      return nullptr;
    }
    printf("%s", shader.getInfoLog());

    const auto interm = shader.getIntermediate();
    __implIntermToVulkanPipe(shaderPipe, interm, langName);
  }

  return shaderPipe;
}

void *loadShaderPipeAndCompile(const std::vector<std::string> &shadernames) {
  std::vector<std::pair<std::vector<uint8_t>, EShLanguage>> vector;
  vector.reserve(shadernames.size());
  for (const auto &name : shadernames) {
    const std::string abrivation = name.substr(name.size() - 4);
    auto path = fs::path(name);
    vector.push_back(std::pair(util::wholeFile(path), getLang(abrivation)));
  }
  const auto loadedPipes = __implLoadShaderPipeAndCompile(vector);
  return (uint8_t *)loadedPipes;
}

constexpr PipelineInputAssemblyStateCreateInfo
    inputAssemblyCreateInfo({}, PrimitiveTopology::eTriangleList,
                            false); // For now constexpr

const std::array shaderNames = {
    std::vector({"testvec4.vert"_str, "test.frag"_str}),
    std::vector({"testUV.vert"_str, "testTexture.frag"_str})};

void *VulkanGraphicsModule::loadShader(const MaterialType type) {
  const auto idx = (size_t)type;
  if (shaderPipes.size() > idx) {
    const auto pipe = shaderPipes[idx];
    if (pipe != nullptr)
      return pipe;
  } else {
    shaderPipes.resize(idx + 1);
  }
  auto &vert = shaderNames[idx];
  const auto ptr = loadShaderPipeAndCompile(vert);
  shaderPipes[idx] = (VulkanShaderPipe *)ptr;
  return ptr;
}

size_t VulkanGraphicsModule::pushMaterials(const size_t materialcount,
                                           const Material *materials) {
  if (materialcount == 0)
    return -1;
  const Rect2D scissor({0, 0},
                       {(uint32_t)viewport.width, (uint32_t)viewport.height});
  const PipelineViewportStateCreateInfo pipelineViewportCreateInfo({}, viewport,
                                                                   scissor);

  const PipelineMultisampleStateCreateInfo multisampleCreateInfo(
      {}, SampleCountFlagBits::e1, false, 1);

  const PipelineColorBlendAttachmentState blendAttachment(
      true, BlendFactor::eSrcAlpha, BlendFactor::eOneMinusSrcAlpha,
      BlendOp::eAdd, BlendFactor::eOne, BlendFactor::eZero, BlendOp::eAdd,
      (ColorComponentFlags)FlagTraits<ColorComponentFlagBits>::allFlags);

  const PipelineColorBlendStateCreateInfo colorBlendState(
      {}, false, LogicOp::eClear, 1, &blendAttachment);

  const PipelineDepthStencilStateCreateInfo pipeDepthState(
      {}, true, true, CompareOp::eGreaterOrEqual, false, false, {}, {}, 0, 1);

  std::vector<GraphicsPipelineCreateInfo> pipelineCreateInfos;
  pipelineCreateInfos.reserve(materialcount);
  pipelineLayouts.reserve(materialcount + pipelineLayouts.size());

  for (size_t i = 0; i < materialcount; i++) {
    const auto &material = materials[i];

    const auto shaderPipe = (VulkanShaderPipe *)material.costumShaderData;

    shaderPipe->pipelineShaderStage.clear();
    shaderPipe->pipelineShaderStage.reserve(shaderPipe->shader.size());

    for (const auto &shaderPair : shaderPipe->shader) {
      const auto &shaderData = shaderPair.first;

      const ShaderModuleCreateInfo shaderModuleCreateInfo(
          {}, shaderData.size() * sizeof(uint32_t), shaderData.data());
      const auto shaderModule =
          device.createShaderModule(shaderModuleCreateInfo);
      shaderModules.push_back(shaderModule);
      shaderPipe->pipelineShaderStage.push_back(PipelineShaderStageCreateInfo(
          {}, shaderPair.second, shaderModule, "main"));
    }

    shaderPipe->rasterization.lineWidth = 1;
    shaderPipe->rasterization.depthBiasEnable = false;
    shaderPipe->rasterization.rasterizerDiscardEnable = false;
    shaderPipe->rasterization.cullMode = CullModeFlagBits::eFront;

    std::vector<DescriptorSetLayout> descLayout;
    std::vector<DescriptorPoolSize> descPoolSizes;
    for (const auto &l : shaderPipe->descriptorLayout) {
      const auto descL = device.createDescriptorSetLayout(l);
      descLayout.push_back(descL);
      for (size_t i = 0; i < l.bindingCount; i++) {
        const auto &binding = l.pBindings[i];
        descPoolSizes.push_back(
            {binding.descriptorType, binding.descriptorCount});
      }
    }

    if (!descLayout.empty()) {
      const DescriptorPoolCreateInfo descPoolCreateInfo({}, descLayout.size(),
                                                        descPoolSizes);
      shaderPipe->descPool = device.createDescriptorPool(descPoolCreateInfo);

      const DescriptorSetAllocateInfo descSetAllocInfo(shaderPipe->descPool,
                                                       descLayout);
      shaderPipe->descriptorSets =
          device.allocateDescriptorSets(descSetAllocInfo);
    }

    const PipelineLayoutCreateInfo layoutCreateInfo({}, descLayout);
    shaderPipe->layout = device.createPipelineLayout(layoutCreateInfo);
    pipelineLayouts.push_back(shaderPipe->layout);

    const GraphicsPipelineCreateInfo gpipeCreateInfo(
        {}, shaderPipe->pipelineShaderStage, &shaderPipe->inputStateCreateInfo,
        &inputAssemblyCreateInfo, {}, &pipelineViewportCreateInfo,
        &shaderPipe->rasterization, &multisampleCreateInfo, &pipeDepthState,
        &colorBlendState, {}, shaderPipe->layout, renderpass, 0);
    pipelineCreateInfos.push_back(gpipeCreateInfo);
  }

  const auto piperesult =
      device.createGraphicsPipelines({}, pipelineCreateInfos);
  VERROR(piperesult.result);
  const auto indexOffset = pipelines.size();
  pipelines.resize(indexOffset + piperesult.value.size());
  std::copy(piperesult.value.cbegin(), piperesult.value.cend(),
            pipelines.begin() + indexOffset);
  return indexOffset;
}

void VulkanGraphicsModule::pushRender(const size_t renderInfoCount,
                                      const RenderInfo *renderInfos) {
  const CommandBufferAllocateInfo commandBufferAllocate(
      pool, CommandBufferLevel::eSecondary, 1);
  const CommandBuffer cmdBuf =
      device.allocateCommandBuffers(commandBufferAllocate).back();

  const CommandBufferInheritanceInfo inheritance(renderpass, 0);
  const CommandBufferBeginInfo beginInfo(
      CommandBufferUsageFlagBits::eSimultaneousUse |
          CommandBufferUsageFlagBits::eRenderPassContinue,
      &inheritance);
  cmdBuf.begin(beginInfo);
  for (size_t i = 0; i < renderInfoCount; i++) {
    auto &info = renderInfos[i];

    std::vector<Buffer> vertexBuffer;
    vertexBuffer.reserve(info.vertexBuffer.size());

    for (auto vertId : info.vertexBuffer) {
      vertexBuffer.push_back(bufferList[vertId]);
    }

    if (info.vertexOffsets.size() == 0) {
      std::vector<size_t> offsets(vertexBuffer.size());
      std::fill(offsets.begin(), offsets.end(), 0);
      cmdBuf.bindVertexBuffers(0, vertexBuffer, offsets);
    } else {
      cmdBuf.bindVertexBuffers(0, vertexBuffer, info.vertexOffsets);
    }

    const auto &material = materials[info.materialId];
    const auto shaderPipe = (VulkanShaderPipe *)material.costumShaderData;
    if (!shaderPipe->descriptorSets.empty()) {
      cmdBuf.bindDescriptorSets(PipelineBindPoint::eGraphics,
                                shaderPipe->layout, 0,
                                shaderPipe->descriptorSets, {});
    }

    cmdBuf.bindIndexBuffer(bufferList[info.indexBuffer], info.indexOffset,
                           (IndexType)info.indexSize);

    cmdBuf.bindPipeline(PipelineBindPoint::eGraphics,
                        pipelines[info.materialId]);

    cmdBuf.drawIndexed(info.indexCount, info.instanceCount, 0, 0, 0);
  }
  cmdBuf.end();
  const std::lock_guard onExitUnlock(commandBufferRecording);
  secondaryCommandBuffer.push_back(cmdBuf);
}

inline BufferUsageFlags getUsageFlagsFromDataType(const DataType type) {
  if (type == DataType::VertexIndexData)
    return BufferUsageFlagBits::eVertexBuffer |
           BufferUsageFlagBits::eIndexBuffer;
  return (BufferUsageFlags)(64 << (uint32_t)type);
}

size_t VulkanGraphicsModule::pushData(const size_t dataCount,
                                      const uint8_t **data,
                                      const size_t *dataSizes,
                                      const DataType type) {
  if (dataCount == 0)
    return -1;

  std::vector<DeviceMemory> tempMemory;
  tempMemory.reserve(dataCount);
  std::vector<Buffer> tempBuffer;
  tempBuffer.reserve(dataCount);

  const auto firstIndex = bufferList.size();
  bufferList.reserve(firstIndex + dataCount);
  const auto firstMemIndex = bufferMemoryList.size();
  bufferMemoryList.reserve(firstMemIndex + dataCount);

  const auto cmdBuf = cmdbuffer.back();

  const CommandBufferBeginInfo beginInfo(
      CommandBufferUsageFlagBits::eOneTimeSubmit);
  cmdBuf.begin(beginInfo);

  const BufferUsageFlags bufferUsage = getUsageFlagsFromDataType(type);

  for (size_t i = 0; i < dataCount; i++) {
    const auto size = dataSizes[i];
    const auto dataptr = data[i];

    const BufferCreateInfo bufferCreateInfo(
        {}, size, BufferUsageFlagBits::eTransferSrc, SharingMode::eExclusive);
    const auto intermBuffer = device.createBuffer(bufferCreateInfo);
    tempBuffer.push_back(intermBuffer);
    const auto memRequ = device.getBufferMemoryRequirements(intermBuffer);

    const MemoryAllocateInfo allocInfo(memRequ.size,
                                       memoryTypeHostVisibleCoherent);
    const auto hostVisibleMemory = device.allocateMemory(allocInfo);
    tempMemory.push_back(hostVisibleMemory);
    device.bindBufferMemory(intermBuffer, hostVisibleMemory, 0);
    const auto mappedHandle =
        device.mapMemory(hostVisibleMemory, 0, VK_WHOLE_SIZE);
    memcpy(mappedHandle, dataptr, size);
    device.unmapMemory(hostVisibleMemory);

    const BufferCreateInfo bufferLocalCreateInfo(
        {}, size,
        BufferUsageFlagBits::eTransferDst | BufferUsageFlagBits::eTransferSrc |
            bufferUsage,
        SharingMode::eExclusive);
    const auto localBuffer = device.createBuffer(bufferLocalCreateInfo);
    bufferList.push_back(localBuffer);
    const auto memRequLocal = device.getBufferMemoryRequirements(localBuffer);
    const MemoryAllocateInfo allocLocalInfo(memRequLocal.size,
                                            memoryTypeDeviceLocal);
    const auto localMem = device.allocateMemory(allocLocalInfo);
    device.bindBufferMemory(localBuffer, localMem, 0);
    bufferMemoryList.push_back(localMem);
    bufferSizeList.push_back(size);

    const BufferCopy copyInfo(0, 0, size);
    cmdBuf.copyBuffer(intermBuffer, localBuffer, copyInfo);
  }

  cmdBuf.end();

  submitAndWait(device, queue, cmdBuf);

  for (const auto mem : tempMemory)
    device.freeMemory(mem);
  for (const auto buf : tempBuffer)
    device.destroyBuffer(buf);

  return firstIndex;
}

size_t VulkanGraphicsModule::pushSampler(const SamplerInfo &sampler) {
  const auto position = this->sampler.size();
  const SamplerCreateInfo samplerCreateInfo(
      {}, (Filter)sampler.minFilter, (Filter)sampler.magFilter,
      SamplerMipmapMode::eLinear, (SamplerAddressMode)sampler.uMode,
      (SamplerAddressMode)sampler.vMode, (SamplerAddressMode)sampler.vMode, 0,
      sampler.anisotropy, sampler.anisotropy);
  const auto smplr = device.createSampler(samplerCreateInfo);
  this->sampler.push_back(smplr);
  return position;
}

size_t VulkanGraphicsModule::pushTexture(const size_t textureCount,
                                         const TextureInfo *textures) {
  if (textureCount == 0 || textures == 0)
    throw std::runtime_error("Invalid API usage in Vulkan pushTexture!");

  const size_t firstIndex = textureImages.size();

  std::vector<Buffer> intermBuffers;
  std::vector<DeviceMemory> intermMemorys;
  std::vector<BufferImageCopy> intermCopys;
  intermBuffers.reserve(textureCount);
  intermMemorys.reserve(textureCount);
  intermCopys.reserve(textureCount);

  util::OnExit exitHandle([&] {
    for (auto mem : intermMemorys)
      device.freeMemory(mem);
    for (auto img : intermBuffers)
      device.destroyBuffer(img);
  });

  textureImages.reserve(firstIndex + textureCount);
  textureMemorys.reserve(firstIndex + textureCount);
  textureImageViews.reserve(firstIndex + textureCount);

  const auto cmd = this->cmdbuffer.back();

  const CommandBufferBeginInfo beginInfo({}, {});
  cmd.begin(beginInfo);

  for (size_t i = 0; i < textureCount; i++) {
    const TextureInfo &tex = textures[i];

    const Extent3D ext = {tex.width, tex.height, 1};
    const Format format = Format::eR8G8B8A8Unorm;

    const BufferCreateInfo intermBufferCreate({}, tex.size,
                                              BufferUsageFlagBits::eTransferSrc,
                                              SharingMode::eExclusive, {});
    const auto intermBuffer = device.createBuffer(intermBufferCreate);
    intermBuffers.push_back(intermBuffer);
    const auto memRequIntern = device.getBufferMemoryRequirements(intermBuffer);
    const MemoryAllocateInfo intermMemAllocInfo(memRequIntern.size,
                                                memoryTypeHostVisibleCoherent);
    const auto intermMemory = device.allocateMemory(intermMemAllocInfo);
    intermMemorys.push_back(intermMemory);
    device.bindBufferMemory(intermBuffer, intermMemory, 0);
    const auto handle = device.mapMemory(intermMemory, 0, VK_WHOLE_SIZE, {});
    std::memcpy(handle, tex.data, tex.size);
    device.unmapMemory(intermMemory);

    const ImageCreateInfo imageCreate(
        {}, ImageType::e2D, format, ext, 1, 1, SampleCountFlagBits::e1,
        ImageTiling::eOptimal,
        ImageUsageFlagBits::eTransferDst | ImageUsageFlagBits::eColorAttachment,
        SharingMode::eExclusive, {});
    const auto image = device.createImage(imageCreate);
    textureImages.push_back(image);
    const auto memRequ = device.getImageMemoryRequirements(image);
    const MemoryAllocateInfo memAllocInfo(memRequ.size, memoryTypeDeviceLocal);
    const auto memory = device.allocateMemory(memAllocInfo);
    textureMemorys.push_back(memory);
    device.bindImageMemory(image, memory, 0);

    intermCopys.push_back({0,
                           ext.width,
                           ext.height,
                           {ImageAspectFlagBits::eColor, 0, 0, 1},
                           {},
                           ext});

    const ImageViewCreateInfo imageViewCreateInfo(
        {}, image, ImageViewType::e2D, format, {},
        {ImageAspectFlagBits::eColor, 0, 1, 0, 1});
    const auto imageView = device.createImageView(imageViewCreateInfo);
    textureImageViews.push_back(imageView);

    waitForImageTransition(cmd, ImageLayout::eUndefined,
                           ImageLayout::eTransferDstOptimal, queueIndex, image,
                           imageViewCreateInfo.subresourceRange);

    cmd.copyBufferToImage(intermBuffer, image, ImageLayout::eTransferDstOptimal,
                          intermCopys.back());
  }

  cmd.end();

  const SubmitInfo submitInfo({}, {}, cmd, {});
  queue.submit(submitInfo, commandBufferFence);
  const Result result =
      device.waitForFences(commandBufferFence, true, UINT64_MAX);
  VERROR(result);
  device.resetFences(commandBufferFence);

  return firstIndex;
}

#ifdef DEBUG
VkBool32 debugMessage(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                      VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                      const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                      void *pUserData) {
  if (messageSeverity == (VkDebugUtilsMessageSeverityFlagBitsEXT)
                             DebugUtilsMessageSeverityFlagBitsEXT::eVerbose) {
    return VK_FALSE;
  }
  std::string severity =
      to_string((DebugUtilsMessageSeverityFlagBitsEXT)messageSeverity);
  std::string type = to_string((DebugUtilsMessageTypeFlagBitsEXT)messageTypes);

  printf("[%s][%s]: %s\n", severity.c_str(), type.c_str(),
         pCallbackData->pMessage);
  return VK_TRUE;
}
#endif

main::Error VulkanGraphicsModule::init() {
#pragma region Instance
  const ApplicationInfo applicationInfo(APPLICATION_NAME, APPLICATION_VERSION,
                                        ENGINE_NAME, ENGINE_VERSION,
                                        VK_API_VERSION_1_0);

  const auto layerInfos = enumerateInstanceLayerProperties();
  std::vector<const char *> layerEnabled;
  for (const auto &layer : layerInfos) {
    const auto lname = layer.layerName.data();
    const auto enditr = layerToEnable.end();
    if (std::find_if(layerToEnable.begin(), enditr,
                     [&](auto in) { return strcmp(lname, in) == 0; }) != enditr)
      layerEnabled.push_back(lname);
  }

  const auto extensionInfos = enumerateInstanceExtensionProperties();
  std::vector<const char *> extensionEnabled;
  for (const auto &extension : extensionInfos) {
    const auto lname = extension.extensionName.data();
    const auto enditr = extensionToEnable.end();
    if (std::find_if(extensionToEnable.begin(), enditr,
                     [&](auto in) { return strcmp(lname, in) == 0; }) != enditr)
      extensionEnabled.push_back(lname);
  }

  const InstanceCreateInfo createInfo(
      {}, &applicationInfo, (uint32_t)layerEnabled.size(), layerEnabled.data(),
      (uint32_t)extensionEnabled.size(), extensionEnabled.data());
  this->instance = createInstance(createInfo);

#ifdef DEBUG
  DispatchLoaderDynamic stat;
  stat.vkCreateDebugUtilsMessengerEXT =
      (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          instance, "vkCreateDebugUtilsMessengerEXT");
  const DebugUtilsMessengerCreateInfoEXT debugUtilsMsgCreateInfo(
      {},
      (DebugUtilsMessageSeverityFlagsEXT)
          FlagTraits<DebugUtilsMessageSeverityFlagBitsEXT>::allFlags,
      (DebugUtilsMessageTypeFlagsEXT)
          FlagTraits<DebugUtilsMessageTypeFlagBitsEXT>::allFlags,
      debugMessage);
  debugMessenger = instance.createDebugUtilsMessengerEXT(
      debugUtilsMsgCreateInfo, nullptr, stat);
#endif
#pragma endregion

#pragma region Device
  constexpr auto getScore = [](auto physDevice) {
    const auto properties = physDevice.getProperties();
    return properties.limits.maxImageDimension2D +
           (properties.deviceType == PhysicalDeviceType::eDiscreteGpu ? 1000
                                                                      : 0);
  };

  const auto physicalDevices = this->instance.enumeratePhysicalDevices();
  this->physicalDevice = *std::max_element(
      physicalDevices.begin(), physicalDevices.end(),
      [&](auto p1, auto p2) { return getScore(p1) < getScore(p2); });

  // just one queue for now
  const auto queueFamilys = this->physicalDevice.getQueueFamilyProperties();
  const auto bgnitr = queueFamilys.begin();
  const auto enditr = queueFamilys.end();
  const auto queueFamilyItr = std::find_if(bgnitr, enditr, [](auto queue) {
    return queue.queueFlags & QueueFlagBits::eGraphics;
  });
  if (queueFamilyItr == enditr)
    return main::Error::NO_GRAPHIC_QUEUE_FOUND;

  const auto queueFamilyIndex = (uint32_t)std::distance(bgnitr, queueFamilyItr);
  const auto &queueFamily = *queueFamilyItr;
  std::vector<float> priorities(queueFamily.queueCount);
  std::fill(priorities.begin(), priorities.end(), 0.0f);

  queueIndex = (uint32_t)std::distance(bgnitr, queueFamilyItr);
  const DeviceQueueCreateInfo queueCreateInfo(
      {}, queueIndex, queueFamily.queueCount, priorities.data());

  const auto devextensions =
      physicalDevice.enumerateDeviceExtensionProperties();
  const auto devextEndItr = devextensions.end();
  const auto fndDevExtItr = std::find_if(
      devextensions.begin(), devextEndItr, [](ExtensionProperties prop) {
        return strcmp(prop.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0;
      });
  if (fndDevExtItr == devextEndItr)
    return main::Error::SWAPCHAIN_EXT_NOT_FOUND;

  const char *name = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
  const DeviceCreateInfo deviceCreateInfo({}, 1, &queueCreateInfo, 0, {}, 1,
                                          &name);
  this->device = this->physicalDevice.createDevice(deviceCreateInfo);

  const auto c4Props =
      this->physicalDevice.getFormatProperties(Format::eR8G8B8A8Unorm);
  if (!(c4Props.optimalTilingFeatures &
        FormatFeatureFlagBits::eColorAttachment))
    return main::Error::FORMAT_NOT_SUPPORTED;

#pragma endregion

#pragma region Queue, Surface, Prepipe, MemTypes
  queue = device.getQueue(queueFamilyIndex, queueIndex);

  const auto winM = graphicsModule->getWindowModule();
#ifdef WIN32
  Win32SurfaceCreateInfoKHR surfaceCreateInfo({}, (HINSTANCE)winM->hInstance,
                                              (HWND)winM->hWnd);
  surface = instance.createWin32SurfaceKHR(surfaceCreateInfo);
#endif // WIN32

  if (!physicalDevice.getSurfaceSupportKHR(queueIndex, surface))
    return main::Error::NO_SURFACE_SUPPORT;

  const auto surfaceFormat = physicalDevice.getSurfaceFormatsKHR(surface);
  const auto surfEndItr = surfaceFormat.end();
  const auto surfBeginItr = surfaceFormat.begin();
  const auto fitr =
      std::find_if(surfBeginItr, surfEndItr, [](SurfaceFormatKHR format) {
        return format.format == Format::eB8G8R8A8Unorm;
      });
  if (fitr == surfEndItr)
    return main::Error::FORMAT_NOT_FOUND;
  format = *fitr;

  const auto memoryProperties = physicalDevice.getMemoryProperties();
  const auto memBeginItr = memoryProperties.memoryTypes.begin();
  const auto memEndItr = memoryProperties.memoryTypes.end();

  const auto findMemoryIndex = [&](auto prop) {
    const auto findItr = std::find_if(memBeginItr, memEndItr, [&](auto &type) {
      return type.propertyFlags & (prop);
    });
    return std::distance(memBeginItr, findItr);
  };

  memoryTypeDeviceLocal = findMemoryIndex(MemoryPropertyFlagBits::eDeviceLocal);
  memoryTypeHostVisibleCoherent =
      findMemoryIndex(MemoryPropertyFlagBits::eHostVisible |
                      MemoryPropertyFlagBits::eHostCoherent);

  const auto capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
  viewport = Viewport(0, 0, capabilities.currentExtent.width,
                      capabilities.currentExtent.height, 0, 1);

  const auto presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
  const auto presentModesEndItr = presentModes.end();
  const auto presentModesBeginItr = presentModes.begin();
  auto fndPresentMode = std::find(presentModesBeginItr, presentModesEndItr,
                                  PresentModeKHR::eMailbox);
  if (fndPresentMode == presentModesEndItr) {
    fndPresentMode = std::find(presentModesBeginItr, presentModesEndItr,
                               PresentModeKHR::eImmediate);
    if (fndPresentMode == presentModesEndItr)
      fndPresentMode = presentModesBeginItr;
  }
  const auto presentMode = *fndPresentMode;
#pragma endregion

#pragma region Swapchain
  const SwapchainCreateInfoKHR swapchainCreateInfo(
      {}, surface, 3, format.format, format.colorSpace,
      capabilities.currentExtent, 1, ImageUsageFlagBits::eColorAttachment,
      SharingMode::eExclusive, 0, nullptr,
      SurfaceTransformFlagBitsKHR::eIdentity,
      CompositeAlphaFlagBitsKHR::eOpaque, presentMode, true, nullptr);

  swapchain = device.createSwapchainKHR(swapchainCreateInfo);

  images = device.getSwapchainImagesKHR(swapchain);
#pragma endregion

#pragma region Depth Attachment
  constexpr std::array potentialDepthFormat = {
      Format::eD32Sfloat, Format::eD32SfloatS8Uint, Format::eD24UnormS8Uint,
      Format::eD16Unorm, Format::eD16UnormS8Uint};
  for (const Format pDF : potentialDepthFormat) {
    const FormatProperties fProp = physicalDevice.getFormatProperties(pDF);
    if (fProp.optimalTilingFeatures &
        FormatFeatureFlagBits::eDepthStencilAttachment) {
      depthFormat = pDF;
      break;
    }
  }
  if (depthFormat == Format::eUndefined)
    return main::Error::FORMAT_NOT_FOUND;

  const ImageCreateInfo depthImageCreateInfo(
      {}, ImageType::e2D, depthFormat,
      {(uint32_t)viewport.width, (uint32_t)viewport.height, 1}, 1, 1,
      SampleCountFlagBits::e1, ImageTiling::eOptimal,
      ImageUsageFlagBits::eDepthStencilAttachment);
  depthImage = device.createImage(depthImageCreateInfo);

  const MemoryRequirements imageMemReq =
      device.getImageMemoryRequirements(depthImage);

  const MemoryAllocateInfo memAllocInfo(imageMemReq.size,
                                        memoryTypeDeviceLocal);
  depthImageMemory = device.allocateMemory(memAllocInfo);

  device.bindImageMemory(depthImage, depthImageMemory, 0);

  const ImageSubresourceRange subresourceRange(ImageAspectFlagBits::eDepth, 0,
                                               1, 0, 1);

  const ImageViewCreateInfo depthImageViewCreateInfo(
      {}, depthImage, ImageViewType::e2D, depthFormat, {}, subresourceRange);
  depthImageView = device.createImageView(depthImageViewCreateInfo);
#pragma endregion

#pragma region Renderpass

  const std::array attachments = {
      AttachmentDescription(
          {}, format.format, SampleCountFlagBits::e1, AttachmentLoadOp::eClear,
          AttachmentStoreOp::eStore, AttachmentLoadOp::eDontCare,
          AttachmentStoreOp::eDontCare, ImageLayout::eUndefined,
          ImageLayout::ePresentSrcKHR),
      AttachmentDescription(
          {}, depthFormat, SampleCountFlagBits::e1, AttachmentLoadOp::eDontCare,
          AttachmentStoreOp::eDontCare, AttachmentLoadOp::eClear,
          AttachmentStoreOp::eStore, ImageLayout::eUndefined,
          ImageLayout::ePresentSrcKHR)};

  constexpr std::array colorAttachments = {
      AttachmentReference(0, ImageLayout::eColorAttachmentOptimal)};

  constexpr AttachmentReference depthAttachment(
      1, ImageLayout::eDepthStencilAttachmentOptimal);

  const std::array subpassDescriptions = {
      SubpassDescription({}, PipelineBindPoint::eGraphics, {}, colorAttachments,
                         {}, &depthAttachment)};

  const std::array subpassDependencies = {SubpassDependency(
      0, VK_SUBPASS_EXTERNAL, PipelineStageFlagBits::eAllGraphics,
      PipelineStageFlagBits::eTopOfPipe, (AccessFlagBits)0, (AccessFlagBits)0)};

  const RenderPassCreateInfo renderPassCreateInfo(
      {}, attachments, subpassDescriptions, subpassDependencies);
  renderpass = device.createRenderPass(renderPassCreateInfo);
#pragma endregion

#pragma region CommandBuffer
  imageviews.reserve(images.size());

  const CommandPoolCreateInfo commandPoolCreateInfo(
      CommandPoolCreateFlagBits::eResetCommandBuffer, queueIndex);
  pool = device.createCommandPool(commandPoolCreateInfo);

  const CommandBufferAllocateInfo cmdBufferAllocInfo(
      pool, CommandBufferLevel::ePrimary, (uint32_t)images.size() + 1);
  cmdbuffer = device.allocateCommandBuffers(cmdBufferAllocInfo);
#pragma endregion

#pragma region ImageViews and Framebuffer
  for (auto im : images) {
    const ImageViewCreateInfo imageviewCreateInfo(
        {}, im, ImageViewType::e2D, format.format, ComponentMapping(),
        ImageSubresourceRange(ImageAspectFlagBits::eColor, 0, 1, 0, 1));

    const auto imview = device.createImageView(imageviewCreateInfo);
    imageviews.push_back(imview);

    const std::array attachments = {imview, depthImageView};

    const FramebufferCreateInfo framebufferCreateInfo(
        {}, renderpass, attachments, viewport.width, viewport.height, 1);
    framebuffer.push_back(device.createFramebuffer(framebufferCreateInfo));
  }
#pragma endregion

#pragma region Vulkan Mutex
  const FenceCreateInfo fenceCreateInfo;
  commandBufferFence = device.createFence(fenceCreateInfo);

  const SemaphoreCreateInfo semaphoreCreateInfo;
  waitSemaphore = device.createSemaphore(semaphoreCreateInfo);
  signalSemaphore = device.createSemaphore(semaphoreCreateInfo);
#pragma endregion
  return main::Error::NONE;
}

void VulkanGraphicsModule::tick(double time) {
  auto nextimage =
      device.acquireNextImageKHR(swapchain, UINT64_MAX, waitSemaphore, {});
  VERROR(nextimage.result);

  const auto currentBuffer = cmdbuffer[nextimage.value];
  if (1) { // For now rerecord every tick
    constexpr std::array clearColor = {1.0f, 0.0f, 1.0f, 1.0f};
    const std::array clearValue = {ClearValue(clearColor),
                                   ClearValue(ClearDepthStencilValue(0.0f, 0))};

    const CommandBufferBeginInfo cmdBufferBeginInfo(
        CommandBufferUsageFlagBits::eSimultaneousUse, nullptr);
    currentBuffer.begin(cmdBufferBeginInfo);

    const RenderPassBeginInfo renderPassBeginInfo(
        renderpass, framebuffer[nextimage.value],
        {{0, 0}, {(uint32_t)viewport.width, (uint32_t)viewport.height}},
        clearValue);
    currentBuffer.beginRenderPass(renderPassBeginInfo,
                                  SubpassContents::eSecondaryCommandBuffers);
    const std::lock_guard onExitUnlock(commandBufferRecording);
    currentBuffer.executeCommands(secondaryCommandBuffer);
    currentBuffer.endRenderPass();
    currentBuffer.end();
  }

  const PipelineStageFlags stageFlag = PipelineStageFlagBits::eAllGraphics;
  const SubmitInfo submitInfo(waitSemaphore, stageFlag, currentBuffer,
                              signalSemaphore);

  queue.submit(submitInfo, commandBufferFence);

  const PresentInfoKHR presentInfo(signalSemaphore, swapchain, nextimage.value,
                                   nullptr);
  const Result result = queue.presentKHR(presentInfo);
  VERROR(result);

  const Result waitresult =
      device.waitForFences(commandBufferFence, true, UINT64_MAX);
  VERROR(waitresult);

  currentBuffer.reset();
  device.resetFences(commandBufferFence);
}

void VulkanGraphicsModule::destroy() {
  device.destroyImageView(depthImageView);
  device.freeMemory(depthImageMemory);
  device.destroyImage(depthImage);
  device.destroyFence(commandBufferFence);
  device.destroySemaphore(waitSemaphore);
  device.destroySemaphore(signalSemaphore);
  device.freeCommandBuffers(pool, secondaryCommandBuffer);
  for (auto imag : textureImages)
    device.destroyImage(imag);
  for (auto mem : textureMemorys)
    device.freeMemory(mem);
  for (auto imView : textureImageViews)
    device.destroyImageView(imView);
  for (auto samp : sampler)
    device.destroySampler(samp);
  for (auto pipeLayout : pipelineLayouts)
    device.destroyPipelineLayout(pipeLayout);
  for (auto mem : bufferMemoryList)
    device.freeMemory(mem);
  for (auto buf : bufferList)
    device.destroyBuffer(buf);
  for (auto pipe : pipelines)
    device.destroyPipeline(pipe);
  for (auto shader : shaderModules)
    device.destroyShaderModule(shader);
  device.freeCommandBuffers(pool, cmdbuffer);
  device.destroyCommandPool(pool);
  for (auto framebuff : framebuffer)
    device.destroyFramebuffer(framebuff);
  for (auto imv : imageviews)
    device.destroyImageView(imv);
  device.destroyRenderPass(renderpass);
  device.destroySwapchainKHR(swapchain);
  device.destroy();
  instance.destroySurfaceKHR(surface);
#ifdef DEBUG
  DispatchLoaderDynamic stat;
  stat.vkDestroyDebugUtilsMessengerEXT =
      (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          instance, "vkDestroyDebugUtilsMessengerEXT");
  instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, stat);
#endif
  instance.destroy();
}

APILayer *getNewVulkanModule() { return new VulkanGraphicsModule(); }

} // namespace tge::graphics
