#pragma once

#ifdef WIN32
#include <Windows.h>
#define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL 1
#define VK_USE_PLATFORM_WIN32_KHR 1
#endif // WIN32
#include "../../../public/Module.hpp"
#include "../GameGraphicsModule.hpp"
#include "VulkanShaderModule.hpp"
#include "VulkanShaderPipe.hpp"
#include <vector>
#include <vulkan/vulkan.hpp>

namespace tge::graphics {

using namespace vk;

class VulkanGraphicsModule : public APILayer {

public:
  Instance instance;
  PhysicalDevice physicalDevice;
  Device device;
  SurfaceKHR surface;
  SurfaceFormatKHR format;
  Format depthFormat = Format::eUndefined;
  SwapchainKHR swapchain;
  std::vector<Image> swapchainImages;
  RenderPass renderpass;
  std::vector<ImageView> swapchainImageviews;
  std::vector<Framebuffer> framebuffer;
  CommandPool pool;
  std::vector<CommandBuffer> cmdbuffer;
  std::vector<Pipeline> pipelines;
  Queue queue;
  uint32_t queueFamilyIndex;
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
  std::vector<Sampler> sampler;
  std::vector<Image> textureImages;
  std::vector<std::tuple<DeviceMemory, size_t>> textureMemorys;
  std::vector<ImageView> textureImageViews;
  std::vector<shader::ShaderPipe> shaderPipes;
  std::vector<CommandBuffer> primary = {CommandBuffer()};

  size_t depthImage;
  size_t albedoImage;
  size_t normalImage;
  size_t roughnessImage;
  size_t metallicImage;

  size_t lightPipe;
  size_t lightBindings;
  Material lightMat;

  uint32_t nextImage = 0;

  bool isInitialiazed = false;
  bool exitFailed = false;

  struct Lights {
    glm::vec4 lights[50];
    int lightCount;
  } lights;


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

  void changeData(const size_t bufferIndex, const uint8_t *data,
                  const size_t dataSizes, const size_t offset = 0) override;

  void pushRender(const size_t renderInfoCount,
                  const RenderInfo *renderInfos) override;

  size_t pushSampler(const SamplerInfo &sampler) override;

  size_t pushTexture(const size_t textureCount,
                     const TextureInfo *textures) override;

  void *loadShader(const MaterialType type) override;
};

} // namespace tge::graphics