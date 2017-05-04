#pragma once

#include <iostream>
#include <time.h>
#include <vector>
#include <chrono>
#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>
#include <thread>
#include "Window.cpp"

#define ERROR(ch, erx) cout << "Error occured (" << erx  << ")" << endl << ch << endl;\
_sleep(2000);\
exit(erx);

using namespace std;

void printTime() {
	time_t tim;
	time(&tim);
	tm point;
	gmtime_s(&point, &tim);
	cout << "[" << point.tm_hour << ":" << point.tm_min << ":" << point.tm_sec << "]:";
}

void handel(VkResult rs) {
	if (rs != VK_SUCCESS) {
		cout << "FAILED WITH ERROR: " << rs;
		throw new exception;
	}
}

void featuresPrint(VkPhysicalDeviceFeatures fet) {
	cout << endl << "-------------------------------------------" << endl << endl;
	cout << "Features: " << endl << endl;
	cout << "robustBufferAccess: " << fet.robustBufferAccess << endl;
	cout << "fullDrawIndexUint32: " << fet.fullDrawIndexUint32 << endl;
	cout << "imageCubeArray: " << fet.imageCubeArray << endl;
	cout << "independentBlend: " << fet.independentBlend << endl;
	cout << "geometryShader: " << fet.geometryShader << endl;
	cout << "tessellationShader: " << fet.tessellationShader << endl;
	cout << "sampleRateShading: " << fet.sampleRateShading << endl;
	cout << "dualSrcBlend: " << fet.dualSrcBlend << endl;
	cout << "logicOp: " << fet.logicOp << endl;
	cout << "multiDrawIndirect: " << fet.multiDrawIndirect << endl;
	cout << "drawIndirectFirstInstance: " << fet.drawIndirectFirstInstance << endl;
	cout << "depthClamp: " << fet.depthClamp << endl;
	cout << "depthBiasClamp: " << fet.depthBiasClamp << endl;
	cout << "fillModeNonSolid: " << fet.fillModeNonSolid << endl;
	cout << "depthBounds: " << fet.depthBounds << endl;
	cout << "wideLines: " << fet.wideLines << endl;
	cout << "largePoints: " << fet.largePoints << endl;
	cout << "alphaToOne: " << fet.alphaToOne << endl;
	cout << "multiViewport: " << fet.multiViewport << endl;
	cout << "samplerAnisotropy: " << fet.samplerAnisotropy << endl;
	cout << "textureCompressionETC2: " << fet.textureCompressionETC2 << endl;
	cout << "textureCompressionASTC_LDR: " << fet.textureCompressionASTC_LDR << endl;
	cout << "textureCompressionBC: " << fet.textureCompressionBC << endl;
	cout << "occlusionQueryPrecise: " << fet.occlusionQueryPrecise << endl;
	cout << "pipelineStatisticsQuery: " << fet.pipelineStatisticsQuery << endl;
	cout << "vertexPipelineStoresAndAtomics: " << fet.vertexPipelineStoresAndAtomics << endl;
	cout << "fragmentStoresAndAtomics: " << fet.fragmentStoresAndAtomics << endl;
	cout << "shaderTessellationAndGeometryPointSize: " << fet.shaderTessellationAndGeometryPointSize << endl;
	cout << "shaderImageGatherExtended: " << fet.shaderImageGatherExtended << endl;
	cout << "shaderStorageImageExtendedFormats: " << fet.shaderStorageImageExtendedFormats << endl;
	cout << "shaderStorageImageMultisample: " << fet.shaderStorageImageMultisample << endl;
	cout << "shaderStorageImageReadWithoutFormat: " << fet.shaderStorageImageReadWithoutFormat << endl;
	cout << "shaderStorageImageWriteWithoutFormat: " << fet.shaderStorageImageWriteWithoutFormat << endl;
	cout << "shaderUniformBufferArrayDynamicIndexing: " << fet.shaderUniformBufferArrayDynamicIndexing << endl;
	cout << "shaderSampledImageArrayDynamicIndexing: " << fet.shaderSampledImageArrayDynamicIndexing << endl;
	cout << "shaderStorageBufferArrayDynamicIndexing: " << fet.shaderStorageBufferArrayDynamicIndexing << endl;
	cout << "shaderStorageImageArrayDynamicIndexing: " << fet.shaderStorageImageArrayDynamicIndexing << endl;
	cout << "shaderClipDistance: " << fet.shaderClipDistance << endl;
	cout << "shaderCullDistance: " << fet.shaderCullDistance << endl;
	cout << "shaderFloat64: " << fet.shaderFloat64 << endl;
	cout << "shaderInt64: " << fet.shaderInt64 << endl;
	cout << "shaderInt16: " << fet.shaderInt16 << endl;
	cout << "shaderResourceResidency: " << fet.shaderResourceResidency << endl;
	cout << "shaderResourceMinLod: " << fet.shaderResourceMinLod << endl;
	cout << "sparseBinding: " << fet.sparseBinding << endl;
	cout << "sparseResidencyBuffer: " << fet.sparseResidencyBuffer << endl;
	cout << "sparseResidencyImage2D: " << fet.sparseResidencyImage2D << endl;
	cout << "sparseResidencyImage3D: " << fet.sparseResidencyImage3D << endl;
	cout << "sparseResidency2Samples: " << fet.sparseResidency2Samples << endl;
	cout << "sparseResidency4Samples: " << fet.sparseResidency4Samples << endl;
	cout << "sparseResidency8Samples: " << fet.sparseResidency8Samples << endl;
	cout << "sparseResidency16Samples: " << fet.sparseResidency16Samples << endl;
	cout << "sparseResidencyAliased: " << fet.sparseResidencyAliased << endl;
	cout << "variableMultisampleRate: " << fet.variableMultisampleRate << endl;
	cout << "inheritedQueries: " << fet.inheritedQueries << endl;
	cout << endl << "-------------------------------------------" << endl << endl;
}

void printVersion(int av) {
	cout << VK_VERSION_MAJOR(av) << "." << VK_VERSION_MINOR(av) << "." << VK_VERSION_PATCH(av);
}

void debugOut(uint32_t div_cou, VkPhysicalDevice graka_phy[]) {
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


namespace GLVV {
	VkDevice cdevice = {};
	VkSwapchainKHR SwapChain = {};
	vector<VkImageView> currentImage = {};
	VkShaderModule vermodule;
	VkShaderModule fragmodule;
	VkPipelineLayout pipline_layout;
	VkRenderPass render_pass;
	VkPipeline pipeline;
	vector<VkFramebuffer> framebuffers = {};
	VkCommandPool commandpool = {};
	vector<VkCommandBuffer> commandbuffers = {};
	VkSemaphore renderAvailable;
	VkSemaphore renderEnd;
	VkQueue queue;
	int actual_image_count = 0;
}