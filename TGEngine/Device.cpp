#pragma once

#include "Device.h"

namespace Pipeline {

	using namespace std;

	void createDevice(Device* dev) {
		uint32_t div_cou = 0;
		handel(vkEnumeratePhysicalDevices(*dev->app->instance, &div_cou, nullptr));

		dev->physical_devices.resize(div_cou);

		handel(vkEnumeratePhysicalDevices(*dev->app->instance, &div_cou, dev->physical_devices.data()));

		uint32_t device_in_array = -1;
		uint32_t max_tex_size = 0;

		/*
		 * Auto graphic card detection in beta state
		 */

		for (int i = 0; i < div_cou; i++) {
			cout << "Found ";
			VkPhysicalDeviceProperties* cprops = new VkPhysicalDeviceProperties;
			vkGetPhysicalDeviceProperties(dev->physical_devices[i], cprops);
			cout << i << ". " << cprops->deviceName << endl << "of type ";
			switch (cprops->deviceType) {
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: {
				cout << "Discrete GPU" << endl;
				if (max_tex_size < cprops->limits.maxImageDimension2D) {
					device_in_array = i;
					max_tex_size = cprops->limits.maxImageDimension2D + 1000;
					cout << "Size of texture " << max_tex_size << " using device" << endl;
				}
				else {
					cout << "Found a better one befor" << endl;
					continue;
				}
				cout << "Supported and recommended" << endl;
				continue;
			}
			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: {
				cout << "Integrated GPU" << endl;
				if (max_tex_size < cprops->limits.maxImageDimension2D) {
					device_in_array = i;
					max_tex_size = cprops->limits.maxImageDimension2D;
					cout << "Size of texture" << max_tex_size << " using device" << endl;
				}
				else {
					cout << "Found a better one befor" << endl;
					continue;
				}
				cout << "Supported but not recommended, searching for a better one" << endl;
				continue;
			}
			}
			cout << "Not supported type";
		}

		if (device_in_array < 0)error("No supported device found", -5);

		dev->currentPhysicalDevice = &dev->physical_devices[device_in_array];

		dev->property = new VkPhysicalDeviceProperties;
		vkGetPhysicalDeviceProperties(*dev->currentPhysicalDevice, dev->property);

		dev->memoryprops = new VkPhysicalDeviceMemoryProperties;
		vkGetPhysicalDeviceMemoryProperties(*dev->currentPhysicalDevice, dev->memoryprops);

		dev->khr_capabilities = new VkSurfaceCapabilitiesKHR;
		handel(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*dev->currentPhysicalDevice, *dev->app->KHR, dev->khr_capabilities));

		uint32_t suf_form_cout = 0;
		handel(vkGetPhysicalDeviceSurfaceFormatsKHR(*dev->currentPhysicalDevice, *dev->app->KHR, &suf_form_cout, nullptr));

		vector<VkSurfaceFormatKHR> formats(suf_form_cout);
		handel(vkGetPhysicalDeviceSurfaceFormatsKHR(*dev->currentPhysicalDevice, *dev->app->KHR, &suf_form_cout, formats.data()));

		bool validformat = false;

		for (size_t foc = 0; foc < suf_form_cout; foc++)
		{
			if ((validformat = (formats[foc].format == dev->prefered_format))) {
				dev->color_space = formats[foc].colorSpace;
				break;
			}
		}
		if (!validformat) {
			error("Format not Valid", -2);
		}

		uint32_t Present_mode_count = 0;
		handel(vkGetPhysicalDeviceSurfacePresentModesKHR(*dev->currentPhysicalDevice, *dev->app->KHR, &Present_mode_count, nullptr));

		vector<VkPresentModeKHR> khr_present_mode(Present_mode_count);
		handel(vkGetPhysicalDeviceSurfacePresentModesKHR(*dev->currentPhysicalDevice, *dev->app->KHR, &Present_mode_count, khr_present_mode.data()));

		bool ismodevalid = false;

		for (size_t prm = 0; prm < Present_mode_count; prm++)
		{
			if ((ismodevalid = (khr_present_mode[prm] == dev->present_mode)))break;
		}

		if (!ismodevalid) {
			error("Present mode not available", -1);
		}

		vector<char*> extlays = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		uint32_t extcout = 0;
		handel(vkEnumerateDeviceExtensionProperties(*dev->currentPhysicalDevice, nullptr, &extcout, nullptr));
		vector<VkExtensionProperties> current_valid_ext(extcout);
		handel(vkEnumerateDeviceExtensionProperties(*dev->currentPhysicalDevice, nullptr, &extcout, current_valid_ext.data()));

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
					vc++;
					break;
				}
			}
		}

		/*
		 * Code for finding Queuefamily beta state
		 */

		uint32_t queue = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(*dev->currentPhysicalDevice, &queue, nullptr);

		if (queue < 0)error("No queue family found", -6);

		vector<VkQueueFamilyProperties> properties(queue);
		vkGetPhysicalDeviceQueueFamilyProperties(*dev->currentPhysicalDevice, &queue, properties.data());
		dev->queuFamalieindex = -1;

		for (size_t i = 0; i < queue; i++)
		{
			VkQueueFamilyProperties prop = properties[i];
			cout << "Found queue family " << prop.queueFlags << endl;
			if ((prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT) {
				dev->queuFamalieindex = i;
				dev->queueCount = prop.queueCount;
				cout << "Found valide" << endl;
				break;
			}
		}

		if (dev->queuFamalieindex < 0 || dev->queueCount <= 0)error("No valide queue family found or queue count 0", -7);

		VkDeviceQueueCreateInfo queue_create_info = {};
		queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_info.queueFamilyIndex = dev->queuFamalieindex;
		vector<float> queues(dev->queueCount);
		for (size_t i = 0; i < queues.size(); i++)
		{
			queues[i] = 1.0f;
		}
		queue_create_info.queueCount = queues.size();
		queue_create_info.pQueuePriorities = queues.data();

		VkPhysicalDeviceFeatures features = {};

		VkDeviceCreateInfo div_create_info = {};
		div_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		vector<VkDeviceQueueCreateInfo> queue_create_infos = { queue_create_info };
		div_create_info.pQueueCreateInfos = queue_create_infos.data();
		div_create_info.queueCreateInfoCount = queue_create_infos.size();
		div_create_info.pEnabledFeatures = &features;
		div_create_info.enabledExtensionCount = val_ext.size();
		div_create_info.ppEnabledExtensionNames = val_ext.data();


		VkBool32 isSupported = false;
		handel(vkGetPhysicalDeviceSurfaceSupportKHR(*dev->currentPhysicalDevice, dev->queuFamalieindex, *dev->app->KHR, &isSupported));
		if (!isSupported) {
			error("Presentation  not supported", -3);
		}

		dev->device = new VkDevice;
		handel(vkCreateDevice(*dev->currentPhysicalDevice, &div_create_info, nullptr, dev->device));
	}

	void destroyDevice(Device* dev) {
		vkDestroyDevice(*dev->device, nullptr);
	}

}