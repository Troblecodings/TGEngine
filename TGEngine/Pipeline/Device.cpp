#include "Device.hpp"

VkPhysicalDevice used_physical_device;
VkPhysicalDeviceProperties device_properties;
VkDevice device;
VkQueueFamilyProperties queue_family;
uint32_t queue_index;
VkQueue queue;
VkPhysicalDeviceMemoryProperties memory_properties;

void createDevice(std::vector<char*> extensions_to_enable, std::vector<char*> layers_to_enable) {

	//Query Physical Devices
	uint32_t count;
	last_result = vkEnumeratePhysicalDevices(instance, &count, nullptr);
	HANDEL(last_result);
	std::vector<VkPhysicalDevice> physical_devices(count);
	last_result = vkEnumeratePhysicalDevices(instance, &count, physical_devices.data());
	HANDEL(last_result);

	//Get best Physical Device
	uint32_t points = 0;
	for each(VkPhysicalDevice device in physical_devices) {
		VkPhysicalDeviceProperties c_properties;
		vkGetPhysicalDeviceProperties(device, &c_properties);
		uint32_t c_points = c_properties.limits.maxImageDimension2D + (c_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 1000:0);
		if (c_points > points) {
			points = c_points;
			used_physical_device = device;
			device_properties = c_properties;
		}
	}
	physical_devices.clear();

	//Query Queues
	vkGetPhysicalDeviceQueueFamilyProperties(used_physical_device, &count, nullptr);
	std::vector<VkQueueFamilyProperties> queue_famaily_properties(count);
	vkGetPhysicalDeviceQueueFamilyProperties(used_physical_device, &count, queue_famaily_properties.data());
	count = 0;
	for each (VkQueueFamilyProperties c_queue_family in queue_famaily_properties)
	{
		if ((c_queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT) {
			queue_index = count;
			queue_family = c_queue_family;
			break;
		}
		count++;
	}

	//Set Prioritis
	std::vector<float> priorities(queue_family.queueCount);
	for (size_t i = 0; i < priorities.size(); i++)
	{
		priorities[i] = 1;
	}

	VkDeviceQueueCreateInfo device_queue_create_info = {
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
	    nullptr,
	    0,
	    queue_index,
	    queue_family.queueCount,
		priorities.data(),
	};

	uint32_t size = 0;

	//Validation for the device layers
	std::vector<const char*> enable_layer;
	if (layers_to_enable.size() > 0) {
		last_result = vkEnumerateDeviceLayerProperties(used_physical_device, &count, nullptr);
		HANDEL(last_result)
		std::vector<VkLayerProperties> usable_layers(count);
		last_result = vkEnumerateDeviceLayerProperties(used_physical_device, &count, usable_layers.data());
		HANDEL(last_result)
		for each(VkLayerProperties layer in usable_layers) {
			for each(const char* name in layers_to_enable) {
				if (std::string(layer.layerName).compare(name) == 0) {
					size = enable_layer.size();
					enable_layer.resize(size + 1);
					enable_layer[size] = name;
					OUT_LV_DEBUG(name)
					break;
				}
			}
		}
		layers_to_enable.clear();
		usable_layers.clear();
	}

	size = extensions_to_enable.size();
	extensions_to_enable.resize(size + 1);
	extensions_to_enable[size] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

	//Validation for the device extensions
	std::vector<const char*> enable_extensions;
	if (extensions_to_enable.size() > 0) {
		last_result = vkEnumerateDeviceExtensionProperties(used_physical_device, nullptr, &count, nullptr);
		HANDEL(last_result)
		std::vector<VkExtensionProperties> usable_extensions(count);
		last_result = vkEnumerateDeviceExtensionProperties(used_physical_device, nullptr, &count, usable_extensions.data());
		HANDEL(last_result)
		for each(VkExtensionProperties extension in usable_extensions) {
			for each(const char* name in extensions_to_enable) {
				if (std::string(extension.extensionName).compare(name) == 0) {
					size = enable_extensions.size();
					enable_extensions.resize(size + 1);
					enable_extensions[size] = name;
					OUT_LV_DEBUG(name)
					break;
				}
			}
		}
		extensions_to_enable.clear();
		usable_extensions.clear();
	}

	VkPhysicalDeviceFeatures device_features;
	vkGetPhysicalDeviceFeatures(used_physical_device, &device_features);

	VkPhysicalDeviceFeatures device_features_to_enable = {};
	device_features_to_enable.samplerAnisotropy = device_features.samplerAnisotropy;
	device_features_to_enable.depthClamp = device_features.depthClamp;


	//Make Device
	VkDeviceCreateInfo device_create_info = {
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
	    nullptr,
	    0,
	    1,
	    &device_queue_create_info,
		enable_layer.size(),
		enable_layer.data(),
		enable_extensions.size(),
		enable_extensions.data(),
	    &device_features_to_enable
	};

	last_result = vkCreateDevice(used_physical_device, &device_create_info, nullptr, &device);
	HANDEL(last_result)

	//Get queue
	vkGetDeviceQueue(device, queue_index, 0, &queue);

	VkBool32 isSupported;
	last_result = vkGetPhysicalDeviceSurfaceSupportKHR(used_physical_device, queue_index, window_list[0]->surface, &isSupported);
	HANDEL(last_result)

	if (!isSupported) {
		std::cerr << "Swapchain not Supported with surface" << std::endl;
		_sleep(1000000);
		exit(-1000);
	}

	vkGetPhysicalDeviceMemoryProperties(used_physical_device, &memory_properties);
}

void destroyDevice() {
	vkDestroyDevice(device, nullptr);
}