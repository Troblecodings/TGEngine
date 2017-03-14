// ConsoleApplication1.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "Console.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>
#include <vector>

using namespace std;
using namespace GLVV;

bool debug = true;

void debugOut(uint32_t div_cou,VkPhysicalDevice graka_phy[]) {
	if (debug) {
		for (size_t i = 0; i < div_cou; i++)
		{
			cout << endl;
			cout << "==================================" << endl;
			cout << endl;
			VkPhysicalDevice cdevice = graka_phy[i];
			VkPhysicalDeviceProperties cdivprops;

			vkGetPhysicalDeviceProperties(cdevice, &cdivprops);

			uint32_t av = cdivprops.apiVersion;
			uint32_t dv = cdivprops.driverVersion;

			cout << i + 1 << ". Card " << cdivprops.deviceName << endl;
			cout << "Api version ";
			printVersion(av);
		    cout << endl;
			cout << "Driver version ";
			printVersion(dv);
			cout << endl;
			cout << "Type " << cdivprops.deviceType << endl;
			cout << "ID " << cdivprops.deviceID << endl;

			uint32_t div_qu_prop_count = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(cdevice, &div_qu_prop_count, nullptr);

			VkPhysicalDeviceFeatures features;
			vkGetPhysicalDeviceFeatures(cdevice, &features);

			featuresPrint(features);

			vector<VkQueueFamilyProperties> mem_props = {};
			mem_props.resize(div_qu_prop_count);
			vkGetPhysicalDeviceQueueFamilyProperties(cdevice, &div_qu_prop_count, mem_props.data());

			for (size_t s = 0; s < div_qu_prop_count; s++)
			{
				cout << endl;
				VkQueueFamilyProperties prop_mem = mem_props[s];
				cout << s + 1 << ". Queue Family " << prop_mem.queueCount << endl;
				cout << "Flag " << prop_mem.queueFlags << endl;
				cout << "Image transf W" << prop_mem.minImageTransferGranularity.width << " - H" << prop_mem.minImageTransferGranularity.height << " - D" << prop_mem.minImageTransferGranularity.depth << endl;
				cout << "Valid Bits " << prop_mem.timestampValidBits << endl;
			}

		}
		cout << endl;
		cout << "==================================" << endl;
		cout << endl;
	}
}

void debugLayerAndExtensions() {
	uint32_t layer_count = 0;
	vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

	vector<VkLayerProperties> props = {};
	props.resize(layer_count);
	vkEnumerateInstanceLayerProperties(&layer_count, props.data());	

	if (debug) {
		for (int i = 0; i < layer_count; i++)
		{
			VkLayerProperties cprop = props[i];
			cout << "Found layer property " << cprop.layerName << " - " << cprop.description << endl;
			cout << "Impl_Version ";
			printVersion(cprop.implementationVersion);
			cout << endl;
			cout << "Spec_Version ";
			printVersion(cprop.specVersion);
			cout << endl;

			uint32_t ext_count = 0;
			vkEnumerateInstanceExtensionProperties(cprop.layerName,&ext_count, nullptr);

			vector<VkExtensionProperties> extns = {};
			extns.resize(ext_count);
			vkEnumerateInstanceExtensionProperties(cprop.layerName,&ext_count, extns.data());

			for (int g = 0; g < ext_count; g++)
			{
				cout << endl;
				VkExtensionProperties ex = extns[g];
				cout << g + 1 << ". " << ex.extensionName << " - Version: ";
				printVersion(ex.specVersion);
				cout << endl;
			}

			cout << endl;
		}

		cout << "Extensions: " << endl;

		uint32_t ext_count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, nullptr);

		vector<VkExtensionProperties> extns = {};
		extns.resize(ext_count);
		vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, extns.data());

		for (int g = 0; g < ext_count; g++)
		{
			cout << endl;
			VkExtensionProperties ex = extns[g];
			cout << g + 1 << ". " << ex.extensionName << " - Version: ";
			printVersion(ex.specVersion);
			cout << endl;
		}

		cout << endl;
	}
}

int main()
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan Engine Test";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.pEngineName = "TEngine";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	vector<char*> pon = {
		"VK_LAYER_LUNARG_standard_validation",
		"VK_LAYER_VALVE_steam_overlay"
	};

	for (size_t gh = 0; gh < sizeof(pon); gh++)
	{

	}

	debugLayerAndExtensions();

	VkInstanceCreateInfo InscreatInfo = {};
	InscreatInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InscreatInfo.pApplicationInfo = &appInfo;
	InscreatInfo.enabledLayerCount = pon.size();
	InscreatInfo.ppEnabledLayerNames = pon.data();

	handel("Creating Instance", vkCreateInstance(&InscreatInfo,nullptr,&instance));

	uint32_t div_cou = 0;
	handel("Get graphic card count", vkEnumeratePhysicalDevices(instance, &div_cou, nullptr));

	if(debug)cout << "Found " << div_cou << " graphic card(s)" << endl;

	vector<VkPhysicalDevice> graka_phy = {};
	graka_phy.resize(div_cou);

	handel("Get physical graphic card", vkEnumeratePhysicalDevices(instance,&div_cou,graka_phy.data()));

	debugOut(div_cou,graka_phy.data());

	
	VkPhysicalDevice c_div = graka_phy[0];
		
	VkPhysicalDeviceFeatures features = {};

	float arra[] = {1.0F ,1.0F ,1.0F ,1.0F};

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

	handel("Create Device",vkCreateDevice(c_div, &div_create_info, nullptr, &cdevice));

	if(debug)featuresPrint(features);

	VkQueue queue;
	vkGetDeviceQueue(cdevice,0,0,&queue);

	vkDeviceWaitIdle(cdevice);

	vkDestroyDevice(cdevice, nullptr);
	vkDestroyInstance(instance, nullptr);

	while (true)
	{
		char ths = {};
		cin >> ths;
	}

	return 0;
}