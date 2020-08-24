#include "../../public/pipeline/Device.hpp"
#include "../../public/pipeline/window/Window.hpp"

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
		//TODO reevalute this calculation ... or your life choices
		uint32_t currentPoints = currentDeviceProperties.limits.maxImageDimension2D + (currentDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 1000 : 0);
		if (currentPoints > points) {
			points = currentPoints;
			physicalDevice = physicalDevices[i];
			deviceProperties = currentDeviceProperties;
		}
	}

	//Query Queues
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamailyProperties(count);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, queueFamailyProperties.data());
	count = 0;
	for (VkQueueFamilyProperties currentQueueFamily : queueFamailyProperties) {
		if ((currentQueueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT) {
			queueIndex = count;
			queueFamily = currentQueueFamily;
			break;
		}
		count++;
	}

	//Set Prioritis
	float* priorities = new float[queueFamily.queueCount];
	for (size_t i = 0; i < queueFamily.queueCount; i++) {
		priorities[i] = 1;
	}

	VkDeviceQueueCreateInfo deviceQueueCreateInfo;
	deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueCreateInfo.pNext = nullptr;
	deviceQueueCreateInfo.flags = 0;
	deviceQueueCreateInfo.queueFamilyIndex = queueIndex;
	deviceQueueCreateInfo.queueCount = queueFamily.queueCount;
	deviceQueueCreateInfo.pQueuePriorities = priorities;

	//Make Device
	const char* extensionsEnable[1];
	extensionsEnable[0] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

	//Validation for the device extensions
	uint32_t extensionCount = 0;
	std::vector<const char*> enabledExtensionNames;
	CHECKFAIL(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr));
	enabledExtensionNames.reserve(extensionCount);
	VkExtensionProperties* usableExtensionNames = new VkExtensionProperties[extensionCount];
	CHECKFAIL(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, usableExtensionNames));
	for (uint32_t i = 0; i < extensionCount; i++) {
		VkExtensionProperties extension = usableExtensionNames[i];
		OUT_LV_DEBUG("Available " << extension.extensionName);
		for (const char* name : extensionsEnable) {
			if (strcmp(extension.extensionName, name) == 0) {
				enabledExtensionNames.push_back(name);
				OUT_LV_DEBUG("Active " << name);
				break;
			}
		}

	}

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

	VkPhysicalDeviceFeatures deviceFeaturesEnable = {};
	deviceFeaturesEnable.samplerAnisotropy = deviceFeatures.samplerAnisotropy;
	deviceFeaturesEnable.depthClamp = deviceFeatures.depthClamp;

	VkDeviceCreateInfo deviceCreateInfo;
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pNext = nullptr;
	deviceCreateInfo.flags = 0;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
	deviceCreateInfo.enabledLayerCount = 0;
	deviceCreateInfo.ppEnabledLayerNames = nullptr;
	deviceCreateInfo.enabledExtensionCount = (uint32_t)enabledExtensionNames.size();
	deviceCreateInfo.ppEnabledExtensionNames = enabledExtensionNames.data();
	deviceCreateInfo.pEnabledFeatures = &deviceFeaturesEnable;
	CHECKFAIL(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device));

	//Get queue
	vkGetDeviceQueue(device, queueIndex, 0, &queue);

	VkBool32 isSupported = 1;
	CHECKFAIL(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueIndex, tge::win::windowSurface, &isSupported));

	ASSERT_NONE_NULL(isSupported, "Swapchain not Supported with surface", TG_ERR_SWAPCHAIN_NOT_SUPPORTED);

	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

	FIND_INDEX(vlibDeviceLocalMemoryIndex, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	FIND_INDEX(vlibDeviceHostVisibleCoherentIndex, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
}

void destroyDevice() {
	vkDestroyDevice(device, nullptr);
}