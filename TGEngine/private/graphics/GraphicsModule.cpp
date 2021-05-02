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

class GraphicsModule : public tge::main::Module {

  Instance instance;
  PhysicalDevice physicalDevice;
  Device device;
  SurfaceKHR surface;

  main::Error init();

  void destroy();

  main::Error createWindowAndGetSurface();
};

#ifdef WIN32

LRESULT callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

main::Error GraphicsModule::createWindowAndGetSurface() {
  auto systemHandle = GetModuleHandle(nullptr);

  WNDCLASSEX wndclass;
  FillMemory(&wndclass, sizeof(WNDCLASSEX), 0);
  wndclass.cbSize = sizeof(WNDCLASSEX);
  wndclass.lpfnWndProc = callback;
  wndclass.hInstance = systemHandle;
  wndclass.lpszClassName = ENGINE_NAME;

  auto regWndClass = RegisterClassEx(&wndclass);
  if (!regWndClass)
    return main::Error::COULD_NOT_CREATE_WINDOW_CLASS;

  auto window = CreateWindowEx(0, (LPCSTR)regWndClass, APPLICATION_NAME,
                               WS_BORDER, CW_USEDEFAULT, 0, 100, 100, NULL,
                               NULL, systemHandle, NULL);
  if (!window)
    return main::Error::COULD_NOT_CREATE_WINDOW;

  Win32SurfaceCreateInfoKHR surfaceCreateInfo({}, systemHandle, window);
  surface = instance.createWin32SurfaceKHR(surfaceCreateInfo);
  return main::Error::NONE;
}
#endif // WIN32

main::Error GraphicsModule::init() {

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

  const auto queueFamily = *queueFamilyItr;
  std::vector<float> priorities(queueFamily.queueCount);
  std::fill(priorities.begin(), priorities.end(), 0.0f);

  const DeviceQueueCreateInfo queueCreateInfo(
      {}, (uint32_t)std::distance(bgnitr, queueFamilyItr),
      queueFamily.queueCount, priorities.data());

  const DeviceCreateInfo deviceCreateInfo({}, 1, &queueCreateInfo);
  this->device = this->physicalDevice.createDevice(deviceCreateInfo);

  auto localError = createWindowAndGetSurface();
  if (localError != main::Error::NONE)
    return localError;
  return main::Error::NONE;
}

void GraphicsModule::destroy() {
  device.destroy();
  instance.destroySurfaceKHR(surface);
  instance.destroy();
}

main::Module *getNewModule() { return new GraphicsModule(); }

} // namespace tge::graphics
