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
	lastResult = vkEnumeratePhysicalDevices(instance, &count, nullptr);
	HANDEL(lastResult);
	VkPhysicalDevice*  physical_devices = new VkPhysicalDevice[count];
	lastResult = vkEnumeratePhysicalDevices(instance, &count, physical_devices);
	HANDEL(lastResult);

	//Get best Physical Device
	uint32_t points = 0;
	for (size_t i = 0; i < count; i++)
	{
		VkPhysicalDeviceProperties c_properties;
		vkGetPhysicalDeviceProperties(physical_devices[i], &c_properties);
		uint32_t c_points = c_properties.limits.maxImageDimension2D + (c_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 1000 : 0);
		if (c_points > points) {
			points = c_points;
			used_physical_device = physical_devices[i];
			device_properties = c_properties;
		}
	}
	delete[] physical_devices;

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

	size_t size = 0;

	//Validation for the device layers
	std::vector<const char*> enable_layer;
	if (layers_to_enable.size() > 0) {
		lastResult = vkEnumerateDeviceLayerProperties(used_physical_device, &count, nullptr);
		HANDEL(lastResult)
		std::vector<VkLayerProperties> usable_layers(count);
		lastResult = vkEnumerateDeviceLayerProperties(used_physical_device, &count, usable_layers.data());
		HANDEL(lastResult)
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
		lastResult = vkEnumerateDeviceExtensionProperties(used_physical_device, nullptr, &count, nullptr);
		HANDEL(lastResult)
		std::vector<VkExtensionProperties> usable_extensions(count);
		lastResult = vkEnumerateDeviceExtensionProperties(used_physical_device, nullptr, &count, usable_extensions.data());
		HANDEL(lastResult)
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
		(uint32_t)enable_layer.size(),
		enable_layer.data(),
		(uint32_t)enable_extensions.size(),
		enable_extensions.data(),
	    &device_features_to_enable
	};

	lastResult = vkCreateDevice(used_physical_device, &device_create_info, nullptr, &device);
	HANDEL(lastResult)

	//Get queue
	vkGetDeviceQueue(device, queue_index, 0, &queue);

	VkBool32 isSupported;
	lastResult = vkGetPhysicalDeviceSurfaceSupportKHR(used_physical_device, queue_index, window_list[0]->surface, &isSupported);
	HANDEL(lastResult)

	ASSERT_NONE_NULL(isSupported, "Swapchain not Supported with surface", TG_ERR_SWAPCHAIN_NOT_SUPPORTED)

	vkGetPhysicalDeviceMemoryProperties(used_physical_device, &memory_properties);

	FIND_INDEX(vlib_device_local_memory_index, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
	FIND_INDEX(vlib_device_host_visible_coherent_index, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
}

void destroyDevice() {
	vkDestroyDevice(device, nullptr);
}