#include "../../public/graphics/VulkanGraphicsModule.hpp"

#include "../../public/Error.hpp"
#include <array>
#include <glslang/Include/intermediate.h>
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>

#include <iostream>

#ifdef WIN32
#include <Windows.h>
#define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL 0
#define VK_USE_PLATFORM_WIN32_KHR 1
#endif // WIN32
#include "../../public/graphics/GameGraphicsModule.hpp"

#include "../../public/Util.hpp"
#include <vector>
#define VULKAN_HPP_HAS_SPACESHIP_OPERATOR
#include <unordered_set>
#include <vulkan/vulkan.hpp>

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

constexpr std::array extensionToEnable = {"VK_KHR_SURFACE_EXTENSION_NAME",
                                          "VK_KHR_surface"
#ifdef WIN32
                                          ,
                                          "VK_KHR_win32_surface"
#endif // WIN32

};

using namespace vk;

std::vector<GraphicsPipelineCreateInfo> pipelineCreateInfos;
Result verror = Result::eSuccess;

#define VERROR(rslt)                                                           \
  if (rslt != Result::eSuccess) {                                              \
    verror = rslt;                                                             \
    main::error = main::Error::VULKAN_ERROR;                                   \
    printf("Vulkan error %d!", (uint32_t)verror);                              \
  }

class VulkanGraphicsModule : public tge::main::Module, public APILayer {
public:
  VulkanGraphicsModule() : gamegraphics(GameGraphicsModule(this)){};

private:
  Instance instance;
  PhysicalDevice physicalDevice;
  Device device;
  SurfaceKHR surface;
  char *window = nullptr;
  SurfaceFormatKHR format;
  SwapchainKHR swapchain;
  std::vector<Image> images;
  RenderPass renderpass;
  std::vector<ImageView> imageviews;
  Framebuffer framebuffer;
  CommandPool pool;
  std::vector<CommandBuffer> cmdbuffer;
  std::vector<Pipeline> pipelines;
  Queue queue;
  Semaphore waitSemaphore;
  Semaphore signalSemaphore;
  GameGraphicsModule gamegraphics;
  std::vector<ShaderModule> shaderModules;

  main::Error init();

  void tick(double time);

  void destroy();

  main::Error createWindowAndGetSurface();

  main::Error pushMaterials(const size_t materialcount,
                            const Material *materials);

  GameGraphicsModule *getGraphicsModule() { return &gamegraphics; }
};

struct CustomeVulkanShader {
  ShaderStageFlagBits language;
  std::vector<uint32_t> data;
  uint8_t *costumData;
};

inline EShLanguage getLang(std::string &str) {
  if (str.compare("vert") == 0)
    return EShLanguage::EShLangVertex;
  if (str.compare("frag") == 0)
    return EShLanguage::EShLangFragment;
  return EShLangAnyHit;
}

inline ShaderStageFlagBits getStageFromLang(const EShLanguage lang) {
  switch (lang) {
  case EShLanguage::EShLangVertex:
    return ShaderStageFlagBits::eVertex;
  case EShLanguage::EShLangFragment:
    return ShaderStageFlagBits::eFragment;
  }
  return ShaderStageFlagBits::eAll;
}

inline Format getFormatFromElf(glslang::TLayoutFormat format) {
  switch (format) {
  default:
    break;
  }
}

struct ShaderAnalizer : public glslang::TIntermTraverser {

  ShaderAnalizer() : glslang::TIntermTraverser(false, true) {
    inputs.reserve(10);
  }

  struct Input {
    uint32_t layoutLocation;
    uint32_t binding;
    Format format;

    bool operator==(const Input &in) const noexcept {
      return in.binding == binding && in.format == format &&
             in.layoutLocation == layoutLocation;
    }
  };
  struct InputHash {
    std::size_t operator()(const Input &s) const noexcept {
      std::size_t h1 = std::hash<uint32_t>{}(s.binding);
      std::size_t h2 = std::hash<uint32_t>{}(s.layoutLocation);
      std::size_t h3 = std::hash<uint64_t>{}((uint64_t)s.format);
      return (h1 ^ (h2 << 1)) ^ (h3 << 2);
    }
  };
  std::unordered_set<Input, InputHash> inputs;

#define NO_BINDING_GIVEN 65535
#define NO_LAYOUT_GIVEN 4095

  void visitSymbol(glslang::TIntermSymbol *symbol) {
    const auto &qualifier = symbol->getQualifier();
    if (qualifier.layoutLocation < 4095) {
      if (qualifier.storage == glslang::TStorageQualifier::EvqVaryingIn) {
        printf("Format %d\n");
        inputs.emplace(
            Input{qualifier.layoutLocation, qualifier.layoutBinding});
      }
    }
  }
};

uint8_t *loadShaderPipeAndCompile(std::vector<std::string> &shadernames) {
  CustomeVulkanShader *shaderArray =
      new CustomeVulkanShader[shadernames.size()];
  glslang::InitializeProcess();

  for (size_t i = 0; i < shadernames.size(); i++) {
    const std::string &shname = shadernames[i];
    std::string abrivation = shname.substr(shname.size() - 4);
    fs::path pth(shname);
    const auto langName = getLang(abrivation);
    shaderArray[i].language = getStageFromLang(langName);
    const auto rawInputdata = util::wholeFile(pth);
    if (rawInputdata == nullptr)
      return nullptr;
    glslang::TShader shader(langName);
    shader.setStrings((const char *const *)(&rawInputdata), 1);
    shader.setEnvInput(glslang::EShSourceGlsl, langName,
                       glslang::EShClientVulkan, 100);
    shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
    shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);
    if (!shader.parse(&DefaultTBuiltInResource, 450, true,
                      EShMessages::EShMsgVulkanRules))
      return nullptr;
    printf("%s", shader.getInfoLog());

    shaderArray[i].data = {};
    shaderArray[i].data.reserve(100);
    const auto interm = shader.getIntermediate();
    const auto node = interm->getTreeRoot();
    if (langName == EShLangVertex) { // currently only vertex analization
      ShaderAnalizer analizer;
      node->traverse(&analizer);
    }

    glslang::GlslangToSpv(*interm, shaderArray[i].data);
  }
  glslang::FinalizeProcess();
  return (uint8_t *)shaderArray;
}

#ifdef WIN32

LRESULT CALLBACK callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

main::Error VulkanGraphicsModule::createWindowAndGetSurface() {
  HMODULE systemHandle;
  if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_PIN, nullptr, &systemHandle))
    return main::Error::NO_MODULE_HANDLE;

  const auto windowProperties = gamegraphics.getWindowProperties();

  WNDCLASSEX wndclass;
  FillMemory(&wndclass, sizeof(WNDCLASSEX), 0);
  wndclass.cbSize = sizeof(WNDCLASSEX);
  wndclass.style = CS_ENABLE | CS_OWNDC | CS_HREDRAW;
  wndclass.lpfnWndProc = callback;
  wndclass.hInstance = systemHandle;
  wndclass.lpszClassName = ENGINE_NAME;

  auto regWndClass = RegisterClassEx(&wndclass);
  if (!regWndClass)
    return main::Error::COULD_NOT_CREATE_WINDOW_CLASS;

  auto window = CreateWindowEx(
      WS_EX_APPWINDOW, (LPCSTR)regWndClass, APPLICATION_NAME,
      WS_CLIPSIBLINGS | WS_CAPTION | WS_SIZEBOX | WS_SYSMENU,
      windowProperties.x, windowProperties.y, windowProperties.width,
      windowProperties.height, NULL, NULL, systemHandle, NULL);
  if (!window)
    return main::Error::COULD_NOT_CREATE_WINDOW;
  this->window = (char *)&window;
  ShowWindow(window, SW_SHOW);
  UpdateWindow(window);

  Win32SurfaceCreateInfoKHR surfaceCreateInfo({}, systemHandle, window);
  surface = instance.createWin32SurfaceKHR(surfaceCreateInfo);
  return main::Error::NONE;
}

main::Error VulkanGraphicsModule::pushMaterials(const size_t materialcount,
                                                const Material *materials) {
  for (size_t i = 0; i < materialcount; i++) {
    const auto &material = materials[i];

    const CustomeVulkanShader *shaderRawData = [&] {
      if (material.costumShaderCount != 0) {
        return (const CustomeVulkanShader *)material.costumShaderData;
      } else
        throw std::runtime_error("Currently unsupported!");
    }();
    const auto shaderCount = [&] { return material.costumShaderCount; }();

    const auto firstIndex = shaderModules.size();

    std::vector<PipelineShaderStageCreateInfo> pipelineShaderStage(shaderCount);
    for (size_t j = 0; j < shaderCount; j++) {
      const auto shaderData = shaderRawData[j];

      const ShaderModuleCreateInfo shaderModuleCreateInfo(
          {}, shaderData.data.size() * sizeof(uint32_t),
          shaderData.data.data());
      const auto shaderModule =
          device.createShaderModule(shaderModuleCreateInfo);
      shaderModules.push_back(shaderModule);
      pipelineShaderStage[j] = PipelineShaderStageCreateInfo(
          {}, shaderData.language, shaderModule, "main");
    }

    const GraphicsPipelineCreateInfo gpipeCreateInfo({}, pipelineShaderStage);
    pipelineCreateInfos.push_back(gpipeCreateInfo);
  }
  return main::Error::NONE;
}
#endif // WIN32

main::Error VulkanGraphicsModule::init() {

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

  const auto queueIndex = (uint32_t)std::distance(bgnitr, queueFamilyItr);
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

  queue = device.getQueue(queueFamilyIndex, queueIndex);

  auto localError = createWindowAndGetSurface();
  if (localError != main::Error::NONE)
    return localError;

  if (!physicalDevice.getSurfaceSupportKHR(queueIndex, surface))
    return main::Error::NO_SURFACE_SUPPORT;

  const auto properties = physicalDevice.getProperties();
  const auto surfaceFormat = physicalDevice.getSurfaceFormatsKHR(surface);
  const auto surfEndItr = surfaceFormat.end();
  const auto fitr = std::find_if(
      surfaceFormat.begin(), surfEndItr, [](SurfaceFormatKHR format) {
        return format.format == Format::eB8G8R8A8Unorm;
      });
  if (fitr == surfEndItr)
    return main::Error::FORMAT_NOT_FOUND;
  format = *fitr;

  const auto capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);

  const auto presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
  const auto presentModesEndItr = presentModes.end();
  const auto fndPresentMode = std::find(
      presentModes.begin(), presentModesEndItr, PresentModeKHR::eMailbox);
  if (fndPresentMode == presentModesEndItr) // Maybe relax?
    return main::Error::PRESENTMODE_NOT_FOUND;
  const auto presentMode = *fndPresentMode;

  const SwapchainCreateInfoKHR swapchainCreateInfo(
      {}, surface, 3, format.format, format.colorSpace,
      capabilities.currentExtent, 1, ImageUsageFlagBits::eColorAttachment,
      SharingMode::eExclusive, 0, nullptr,
      SurfaceTransformFlagBitsKHR::eIdentity,
      CompositeAlphaFlagBitsKHR::eOpaque, presentMode, true, nullptr);

  swapchain = device.createSwapchainKHR(swapchainCreateInfo);

  images = device.getSwapchainImagesKHR(swapchain);

  const AttachmentDescription defaultColorAttachment(
      {}, format.format, SampleCountFlagBits::e1, AttachmentLoadOp::eClear,
      AttachmentStoreOp::eStore, AttachmentLoadOp::eDontCare,
      AttachmentStoreOp::eDontCare, ImageLayout::eGeneral,
      ImageLayout::ePresentSrcKHR);
  const std::array attachments = {
      defaultColorAttachment, defaultColorAttachment, defaultColorAttachment};

  const std::array colorAttachments = {
      AttachmentReference(0, ImageLayout::eGeneral),
      AttachmentReference(1, ImageLayout::eGeneral),
      AttachmentReference(2, ImageLayout::eGeneral)};

  const std::array subpassDescriptions = {SubpassDescription(
      {}, PipelineBindPoint::eGraphics, 0, {},
      (uint32_t)colorAttachments.size(), colorAttachments.data())};

  const std::array subpassDependencies = {SubpassDependency(
      VK_SUBPASS_EXTERNAL, 0, PipelineStageFlagBits::eTopOfPipe,
      PipelineStageFlagBits::eAllGraphics, (AccessFlagBits)0,
      AccessFlagBits::eInputAttachmentRead |
          AccessFlagBits::eColorAttachmentRead |
          AccessFlagBits::eColorAttachmentWrite)};

  const RenderPassCreateInfo renderPassCreateInfo(
      {}, (uint32_t)attachments.size(), attachments.data(),
      (uint32_t)subpassDescriptions.size(), subpassDescriptions.data());
  renderpass = device.createRenderPass(renderPassCreateInfo);

  imageviews.reserve(images.size());
  for (auto im : images) {
    const ImageViewCreateInfo imageviewCreateInfo(
        {}, im, ImageViewType::e2D, format.format, {},
        ImageSubresourceRange(ImageAspectFlagBits::eColor, 0, 1, 0, 1));
    const auto imview = device.createImageView(imageviewCreateInfo);
    imageviews.push_back(imview);
  }

  const FramebufferCreateInfo framebufferCreateInfo(
      {}, renderpass, (uint32_t)imageviews.size(), imageviews.data(),
      capabilities.currentExtent.width, capabilities.currentExtent.height, 1);
  framebuffer = device.createFramebuffer(framebufferCreateInfo);

  const CommandPoolCreateInfo commandPoolCreateInfo(
      CommandPoolCreateFlagBits::eResetCommandBuffer, queueIndex);
  pool = device.createCommandPool(commandPoolCreateInfo);

  const CommandBufferAllocateInfo cmdBufferAllocInfo(
      pool, CommandBufferLevel::ePrimary, (uint32_t)imageviews.size());
  cmdbuffer = device.allocateCommandBuffers(cmdBufferAllocInfo);

  main::error = gamegraphics.init();
  if (main::error != main::Error::NONE)
    return main::error;

  const auto piperesult =
      device.createGraphicsPipelines({}, pipelineCreateInfos);
  VERROR(piperesult.result);
  pipelines = piperesult.value;

  const SemaphoreCreateInfo semaphoreCreateInfo;
  waitSemaphore = device.createSemaphore(semaphoreCreateInfo);
  signalSemaphore = device.createSemaphore(semaphoreCreateInfo);

  return main::Error::NONE;
}

void VulkanGraphicsModule::tick(double time) {
  auto nextimage =
      device.acquireNextImageKHR(swapchain, INT64_MAX, waitSemaphore, {});
  VERROR(nextimage.result);

  const PipelineStageFlags stageFlag = PipelineStageFlagBits::eAllGraphics;
  const SubmitInfo submitInfo(1, &waitSemaphore, &stageFlag, 1,
                              &cmdbuffer[nextimage.value], 1, &signalSemaphore);
  queue.submit(submitInfo, {});

  const PresentInfoKHR presentInfo(1, &signalSemaphore, 1, &swapchain,
                                   &nextimage.value, nullptr);
  const Result result = queue.presentKHR(presentInfo);
  VERROR(result);
}

void VulkanGraphicsModule::destroy() {
  device.destroySemaphore(waitSemaphore);
  device.destroySemaphore(signalSemaphore);
  for (auto pipe : pipelines)
    device.destroyPipeline(pipe);
  for (auto shader : shaderModules)
    device.destroyShaderModule(shader);
  device.freeCommandBuffers(pool, cmdbuffer);
  device.destroyCommandPool(pool);
  device.destroyFramebuffer(framebuffer);
  for (auto imv : imageviews)
    device.destroyImageView(imv);
  device.destroyRenderPass(renderpass);
  device.destroySwapchainKHR(swapchain);
#ifdef WIN32
  DestroyWindow((HWND)(*this->window));
#endif // WIN32
  device.destroy();
  instance.destroySurfaceKHR(surface);
  instance.destroy();
}

main::Module *getNewModule() { return new VulkanGraphicsModule(); }

} // namespace tge::graphics
