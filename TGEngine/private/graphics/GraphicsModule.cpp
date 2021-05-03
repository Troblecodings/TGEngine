#include "../../public/graphics/GraphicsModule.hpp"

#include "../../public/Error.hpp"
#include <iostream>
#ifdef WIN32
#include <Windows.h>
#define VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL 0
#define VK_USE_PLATFORM_WIN32_KHR
#endif // WIN32
#include <vulkan/vulkan.hpp>

namespace tge::graphics {

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

class VulkanGraphicsModule : public tge::main::Module {

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

  main::Error init();

  void tick(double time);

  void destroy();

  main::Error createWindowAndGetSurface();
};

#ifdef WIN32

LRESULT CALLBACK callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

main::Error VulkanGraphicsModule::createWindowAndGetSurface() {
  HMODULE systemHandle;
  if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_PIN, nullptr, &systemHandle))
    return main::Error::NO_MODULE_HANDLE;

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

  auto window =
      CreateWindowEx(WS_EX_APPWINDOW, (LPCSTR)regWndClass, APPLICATION_NAME,
                     WS_CLIPSIBLINGS | WS_CAPTION | WS_SIZEBOX | WS_SYSMENU, 0,
                     0, 800, 800, NULL, NULL, systemHandle, NULL);
  if (!window)
    return main::Error::COULD_NOT_CREATE_WINDOW;
  this->window = (char *)&window;
  ShowWindow(window, SW_SHOW);
  UpdateWindow(window);

  Win32SurfaceCreateInfoKHR surfaceCreateInfo({}, systemHandle, window);
  surface = instance.createWin32SurfaceKHR(surfaceCreateInfo);
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

  const uint32_t queueFamilyIndex = std::distance(bgnitr, queueFamilyItr);
  const auto queueFamily = *queueFamilyItr;
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

  const std::array subpassDescriptions = {
      SubpassDescription({}, PipelineBindPoint::eGraphics, 0, {},
                         colorAttachments.size(), colorAttachments.data())};

  const std::array subpassDependencies = {SubpassDependency(
      VK_SUBPASS_EXTERNAL, 0, PipelineStageFlagBits::eTopOfPipe,
      PipelineStageFlagBits::eAllGraphics, (AccessFlagBits)0,
      AccessFlagBits::eInputAttachmentRead |
          AccessFlagBits::eColorAttachmentRead |
          AccessFlagBits::eColorAttachmentWrite)};

  const RenderPassCreateInfo renderPassCreateInfo(
      {}, attachments.size(), attachments.data(), subpassDescriptions.size(),
      subpassDescriptions.data());
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
      {}, renderpass, imageviews.size(), imageviews.data(),
      capabilities.currentExtent.width, capabilities.currentExtent.height, 1);
  framebuffer = device.createFramebuffer(framebufferCreateInfo);

  const CommandPoolCreateInfo commandPoolCreateInfo(
      CommandPoolCreateFlagBits::eResetCommandBuffer, queueIndex);
  pool = device.createCommandPool(commandPoolCreateInfo);

  const CommandBufferAllocateInfo cmdBufferAllocInfo(
      pool, CommandBufferLevel::ePrimary, imageviews.size());
  cmdbuffer = device.allocateCommandBuffers(cmdBufferAllocInfo);

  const auto piperesult = device.createGraphicsPipelines({}, pipelineCreateInfos);
  VERROR(piperesult.result);
  pipelines = piperesult.value;

  const SemaphoreCreateInfo semaphoreCreateInfo;
  device.createSemaphore(semaphoreCreateInfo);

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

  const PresentInfoKHR presentInfo(1, &signalSemaphore, 1, &swapchain, &nextimage.value, nullptr);
  const Result result = queue.presentKHR(presentInfo);
  VERROR(result);
}

void VulkanGraphicsModule::destroy() {
  device.destroySemaphore(waitSemaphore);
  device.destroySemaphore(signalSemaphore);
  for (auto pipe : pipelines)
    device.destroyPipeline(pipe);
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
