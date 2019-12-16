#include "Device.hpp"
#include "window/Window.hpp"

VkPhysicalDevice physicalDevice;
VkPhysicalDeviceProperties deviceProperties;
VkDevice device;
VkQueueFamilyProperties queueFamily;
uint32_t queueIndex;
VkQueue queue;
VkPhysicalDeviceMemoryProperties memoryProperties;

void createDevice() {

	//Query Physical Devices
	uint32_t count;
	CHECKFAIL(vkEnumeratePhysicalDevices(instance, &count, nullptr));
		VkPhysicalDevice* physicalDevices = new VkPhysicalDevice[count];
	CHECKFAIL(vkEnumeratePhysicalDevices(instance, &count, physicalDevices));
		//Get best Physical Device
	uint32_t points = 0;
	for (size_t i = 0; i < count; i++) {
		VkPhysicalDeviceProperties currentDeviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevices[i], &currentDeviceProperties);
		// TODO reevalute this calculation ... or you life choices
		uint32_t currentPoints = currentDeviceProperties.limits.maxImageDimension2D + (currentDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 1000 : 0);
		if (currentPoints > points) {
			points = currentPoints;
			physicalDevice = physicalDevices[i];
			deviceProperties = currentDeviceProperties;
		}
	}
	delete[] physicalDevices;

	//Query Queues
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamailyProperties(count);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, queueFamailyProperties.data());
	count = 0;
	for each (VkQueueFamilyProperties currentQueueFamily in queueFamailyProperties) {
		if ((currentQueueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT) {
			queueIndex = count;
			queueFamily = currentQueueFamily;
			break;
		}
		count++;
	}

	//Set Prioritis
	std::vector<float> priorities(queueFamily.queueCount);
	for (size_t i = 0; i < priorities.size(); i++) {
		priorities[i] = 1;
	}

	VkDeviceQueueCreateInfo deviceQueueCreateInfo = {
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		nullptr,
		0,
		queueIndex,
		queueFamily.queueCount,
		priorities.data(),
	};

	//Make Device
	VkDeviceCreateInfo deviceCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
	deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
	deviceCreateInfo.queueCreateInfoCount = 1;
#if 0
	//Validation for the device layers
	std::vector<const char*> enabledLayer;
	const char* layersEnable[0];
	CHECKFAIL(vkEnumerateDeviceLayerProperties(physicalDevice, &count, nullptr));
	std::vector<VkLayerProperties> usableLayers(count);
	CHECKFAIL(vkEnumerateDeviceLayerProperties(physicalDevice, &count, usableLayers.data()));
	for each (VkLayerProperties layer in usableLayers) {
		for each (const char* name in layersEnable) {
			if (strcmp(layer.layerName, name) == 0) {
				enabledLayer.push_back(name);
				OUT_LV_DEBUG(name)
					break;
			}
		}
	}
	usableLayers.clear();
	deviceCreateInfo.enabledLayerCount = enabledLayer.maximumIndexCount();
	deviceCreateInfo.ppEnabledLayerNames = enabledLayer.data();
#endif // Disable device layer

#if 1
	const char* extensionsEnable[1];
	extensionsEnable[0] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

	//Validation for the device extensions
	std::vector<const char*> enabledExtensions;
	CHECKFAIL(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &count, nullptr));
	std::vector<VkExtensionProperties> usableExtensions(count);
	CHECKFAIL(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &count, usableExtensions.data()));
	for each (VkExtensionProperties extension in usableExtensions) {
		for each (const char* name in extensionsEnable) {
			if (strcmp(extension.extensionName, name) == 0) {
				enabledExtensions.push_back(name);
				OUT_LV_DEBUG(name)
					break;
			}
		}
	}
	usableExtensions.clear();
	deviceCreateInfo.enabledExtensionCount = enabledExtensions.size();
	deviceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
#endif // Enabled extensions

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

	VkPhysicalDeviceFeatures deviceFeaturesEnable = {};
	deviceFeaturesEnable.samplerAnisotropy = deviceFeatures.samplerAnisotropy;
	deviceFeaturesEnable.depthClamp = deviceFeatures.depthClamp;

	deviceCreateInfo.pEnabledFeatures = &deviceFeaturesEnable;
	CHECKFAIL(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device));

	//Get queue
	vkGetDeviceQueue(device, queueIndex, 0, &queue);

	VkBool32 isSupported = 1;
	CHECKFAIL(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueIndex, tge::win::windowSurface, &isSupported));

	ASSERT_NONE_NULL(isSupported, "Swapchain not Supported with surface", TG_ERR_SWAPCHAIN_NOT_SUPPORTED)

		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

	FIND_INDEX(vlibDeviceLocalMemoryIndex, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
		FIND_INDEX(vlibDeviceHostVisibleCoherentIndex, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
}

void destroyDevice() {
	vkDestroyDevice(device, nullptr);
}