#pragma once

#ifdef WIN32
#include <Windows.h>
#define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL 1
#define VK_USE_PLATFORM_WIN32_KHR 1
#endif // WIN32
#include "../../public/Module.hpp"
#include "../../public/graphics/VulkanShaderModule.hpp"
#include "../../public/graphics/VulkanShaderPipe.hpp"
#include "GameGraphicsModule.hpp"
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
  Image depthImage;
  DeviceMemory depthImageMemory;
  ImageView depthImageView;
  std::vector<PipelineLayout> pipelineLayouts;
  std::vector<Sampler> sampler;
  std::vector<Image> textureImages;
  std::vector<DeviceMemory> textureMemorys;
  std::vector<ImageView> textureImageViews;
  std::vector<tge::shader::VulkanShaderPipe *> shaderPipes;
  std::vector<std::vector<DescriptorSet>> descriptorSets;
  std::vector<DescriptorPool> descriptorPoolInfos;
  std::vector<DescriptorSetLayout> descSetLayouts;

  bool isInitialiazed = false;
  bool exitFailed = false;

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

} // namespace tge::graphics