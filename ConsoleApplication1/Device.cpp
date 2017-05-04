#include "stdafx.h"
#include "Console.h"
#include "Application.cpp"


namespace DEVICE {
	struct Device {
		VkFormat prefered_format;
		vector<VkPhysicalDevice> physical_devices;
		VkSurfaceCapabilitiesKHR* khr_capabilities;
		virtual VkPhysicalDevice getGraphicCard() { return physical_devices[0]; }
		VkPhysicalDevice currentPhysicalDevice;
		VkColorSpaceKHR color_space;
		VkDevice* device;
		APP::Application app;
	};

	Device current_device;
}

void createDevice(DEVICE::Device dev) {
	uint32_t div_cou = 0;
	handel(vkEnumeratePhysicalDevices(*dev.app.instance, &div_cou, nullptr));

	dev.physical_devices.resize(div_cou);

	handel(vkEnumeratePhysicalDevices(*dev.app.instance, &div_cou, dev.physical_devices.data()));

	dev.currentPhysicalDevice = dev.getGraphicCard();

	handel(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dev.currentPhysicalDevice, *dev.app.KHR, dev.khr_capabilities));

	uint32_t suf_form_cout = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(dev.currentPhysicalDevice, *dev.app.KHR, &suf_form_cout, nullptr);

	vector<VkSurfaceFormatKHR> formats = {};
	formats.resize(suf_form_cout);
	vkGetPhysicalDeviceSurfaceFormatsKHR(dev.currentPhysicalDevice, *dev.app.KHR, &suf_form_cout, formats.data());

	bool validformat = false;

	for (size_t foc = 0; foc < suf_form_cout; foc++)
	{
		if ((validformat = (formats[foc].format == dev.prefered_format))) {
			dev.color_space = formats[foc].colorSpace;
			break;
		}
	}
	if (!validformat) {
		ERROR("Format not Valid", -2)
	}

	uint32_t Present_mode_count = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(dev.currentPhysicalDevice, *dev.app.KHR, &Present_mode_count, nullptr);

	vector<VkPresentModeKHR> khr_present_mode = {};
	khr_present_mode.resize(Present_mode_count);
	vkGetPhysicalDeviceSurfacePresentModesKHR(dev.currentPhysicalDevice, *dev.app.KHR, &Present_mode_count, khr_present_mode.data());

	bool ismodevalid = false;

	for (size_t prm = 0; prm < Present_mode_count; prm++)
	{
		if ((ismodevalid = (khr_present_mode[prm] == dev.prefered_format)))break;
	}

	if (!ismodevalid) {
		ERROR("Present mode not available", -1)
	}

	VkPhysicalDeviceFeatures features = {};

	float arra[] = { 1.0F ,1.0F ,1.0F ,1.0F };

	VkDeviceQueueCreateInfo queue_create_info = {};
	queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.queueFamilyIndex = 0;
	queue_create_info.queueCount = 1;
	queue_create_info.pQueuePriorities = arra;

	VkDeviceCreateInfo div_create_info = {};
	div_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	div_create_info.pQueueCreateInfos = &queue_create_info;
	div_create_info.queueCreateInfoCount = 1;
	div_create_info.pEnabledFeatures = &features;

	vector<char*> extlays = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	uint32_t extcout = 0;
	vkEnumerateDeviceExtensionProperties(dev.currentPhysicalDevice, nullptr, &extcout, nullptr);
	vector<VkExtensionProperties> current_valid_ext = {};
	current_valid_ext.resize(extcout);
	vkEnumerateDeviceExtensionProperties(dev.currentPhysicalDevice, nullptr, &extcout, current_valid_ext.data());

	int vc = 0;

	vector<char*> val_ext = {};

	for (size_t gh = 0; gh < extlays.size(); gh++)
	{
		string point = extlays[gh];
		for (size_t cf = 0; cf < extcout; cf++)
		{
			string name = current_valid_ext[cf].extensionName;
			if (name == point) {
				val_ext.resize(vc + 1);
				val_ext[vc] = extlays[gh];
				cout << "Found valid device extension " << extlays[gh] << endl;
				vc++;
				break;
			}
		}
	}
	div_create_info.enabledExtensionCount = val_ext.size();
	div_create_info.ppEnabledExtensionNames = val_ext.data();

	VkBool32 isSupported = false;
	handel(vkGetPhysicalDeviceSurfaceSupportKHR(dev.currentPhysicalDevice, 0, *dev.app.KHR, &isSupported));
	if (!isSupported) {
		ERROR("Swapchain not supported", -3);
	}

	handel(vkCreateDevice(dev.currentPhysicalDevice, &div_create_info, nullptr, dev.device));
}

void destroyDevice(DEVICE::Device dev) {
	vkDestroyDevice(*dev.device,nullptr);
}