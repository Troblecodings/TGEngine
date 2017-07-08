#pragma once

#include "stdbase.h"
#include "Application.h"

namespace Pipeline {

	using namespace std;

	struct Device {
		VkFormat prefered_format;
		vector<VkPhysicalDevice> physical_devices;
		VkSurfaceCapabilitiesKHR* khr_capabilities;
		virtual VkPhysicalDevice getGraphicCard() { return physical_devices[0]; }
		VkPhysicalDevice currentPhysicalDevice;
		VkColorSpaceKHR color_space;
		VkDevice* device;
		Application app;
	};

	/*
	 * Creates a Device
	 */
	void createDevice(Device dev);

	/*
	 * Destroys the Device
	 */
	void destroyDevice(Device dev);
}