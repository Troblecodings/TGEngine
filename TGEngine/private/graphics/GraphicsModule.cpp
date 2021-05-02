#include "../../public/graphics/GraphicsModule.hpp"

#include "../../public/Error.hpp"
#include <iostream>
#include <vulkan/vulkan.hpp>

namespace tge::graphics {

constexpr std::array layerToEnable = {"VK_LAYER_LUNARG_standard_validation",
                                      "VK_LAYER_VALVE_steam_overlay",
                                      "VK_LAYER_NV_optimus"};

constexpr std::array extensionToEnable = {"VK_KHR_SURFACE_EXTENSION_NAME"};

using namespace vk;

class GraphicsModule : public tge::main::Module {

  Instance vkInstance;
  PhysicalDevice physicalDevice;
  Device device;

  main::Error init();

  void destroy();
};

main::Error GraphicsModule::init() {

  const ApplicationInfo applicationInfo(APPLICATION_NAME, APPLICATION_VERSION,
                                        ENGINE_NAME, ENGINE_VERSION,
                                        VK_API_VERSION_1_0);

  const auto layerInfos = enumerateInstanceLayerProperties();
  std::vector<const char *> layerEnabled;
  for (const auto &layer : layerInfos) {
    const auto lname = layer.layerName.data();
    const auto enditr = layerToEnable.end();
    if (std::find(layerToEnable.begin(), enditr, lname) != enditr)
      layerEnabled.push_back(lname);
  }

  const auto extensionInfos = enumerateInstanceExtensionProperties();
  std::vector<const char *> extensionEnabled;
  for (const auto &extension : extensionInfos) {
    const auto lname = extension.extensionName.data();
    const auto enditr = extensionToEnable.end();
    if (std::find(extensionToEnable.begin(), enditr, lname) != enditr)
      extensionEnabled.push_back(lname);
  }

  const InstanceCreateInfo createInfo(
      {}, &applicationInfo, (uint32_t)layerEnabled.size(), layerEnabled.data(),
      (uint32_t)extensionEnabled.size(), extensionEnabled.data());
  this->vkInstance = createInstance(createInfo);

  constexpr auto getScore = [](auto physDevice) {
    const auto properties = physDevice.getProperties();
    return properties.limits.maxImageDimension2D +
           (properties.deviceType == PhysicalDeviceType::eDiscreteGpu ? 1000
                                                                      : 0);
  };

  const auto physicalDevices = this->vkInstance.enumeratePhysicalDevices();
  this->physicalDevice =
      *std::max_element(physicalDevices.begin(), physicalDevices.end(),
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

  return main::Error::NONE;
}

void GraphicsModule::destroy() {
  vkInstance.destroy();
  device.destroy();
}

main::Module *getNewModule() { return new GraphicsModule(); }

} // namespace tge::graphics
