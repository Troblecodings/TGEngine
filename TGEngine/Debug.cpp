#include "Debug.h"
#include <time.h>
#include <chrono>

using namespace std;

void printFeatures(VkPhysicalDeviceFeatures device_features) {
	cout << endl << "-------------------------------------------" << endl << endl;
	cout << "Features: " << endl << endl;
	cout << "robustBufferAccess: " << device_features.robustBufferAccess << endl;
	cout << "fullDrawIndexUint32: " << device_features.fullDrawIndexUint32 << endl;
	cout << "imageCubeArray: " << device_features.imageCubeArray << endl;
	cout << "independentBlend: " << device_features.independentBlend << endl;
	cout << "geometryShader: " << device_features.geometryShader << endl;
	cout << "tessellationShader: " << device_features.tessellationShader << endl;
	cout << "sampleRateShading: " << device_features.sampleRateShading << endl;
	cout << "dualSrcBlend: " << device_features.dualSrcBlend << endl;
	cout << "logicOp: " << device_features.logicOp << endl;
	cout << "multiDrawIndirect: " << device_features.multiDrawIndirect << endl;
	cout << "drawIndirectFirstInstance: " << device_features.drawIndirectFirstInstance << endl;
	cout << "depthClamp: " << device_features.depthClamp << endl;
	cout << "depthBiasClamp: " << device_features.depthBiasClamp << endl;
	cout << "fillModeNonSolid: " << device_features.fillModeNonSolid << endl;
	cout << "depthBounds: " << device_features.depthBounds << endl;
	cout << "wideLines: " << device_features.wideLines << endl;
	cout << "largePoints: " << device_features.largePoints << endl;
	cout << "alphaToOne: " << device_features.alphaToOne << endl;
	cout << "multiViewport: " << device_features.multiViewport << endl;
	cout << "samplerAnisotropy: " << device_features.samplerAnisotropy << endl;
	cout << "textureCompressionETC2: " << device_features.textureCompressionETC2 << endl;
	cout << "textureCompressionASTC_LDR: " << device_features.textureCompressionASTC_LDR << endl;
	cout << "textureCompressionBC: " << device_features.textureCompressionBC << endl;
	cout << "occlusionQueryPrecise: " << device_features.occlusionQueryPrecise << endl;
	cout << "pipelineStatisticsQuery: " << device_features.pipelineStatisticsQuery << endl;
	cout << "vertexPipelineStoresAndAtomics: " << device_features.vertexPipelineStoresAndAtomics << endl;
	cout << "fragmentStoresAndAtomics: " << device_features.fragmentStoresAndAtomics << endl;
	cout << "shaderTessellationAndGeometryPointSize: " << device_features.shaderTessellationAndGeometryPointSize << endl;
	cout << "shaderImageGatherExtended: " << device_features.shaderImageGatherExtended << endl;
	cout << "shaderStorageImageExtendedFormats: " << device_features.shaderStorageImageExtendedFormats << endl;
	cout << "shaderStorageImageMultisample: " << device_features.shaderStorageImageMultisample << endl;
	cout << "shaderStorageImageReadWithoutFormat: " << device_features.shaderStorageImageReadWithoutFormat << endl;
	cout << "shaderStorageImageWriteWithoutFormat: " << device_features.shaderStorageImageWriteWithoutFormat << endl;
	cout << "shaderUniformBufferArrayDynamicIndexing: " << device_features.shaderUniformBufferArrayDynamicIndexing << endl;
	cout << "shaderSampledImageArrayDynamicIndexing: " << device_features.shaderSampledImageArrayDynamicIndexing << endl;
	cout << "shaderStorageBufferArrayDynamicIndexing: " << device_features.shaderStorageBufferArrayDynamicIndexing << endl;
	cout << "shaderStorageImageArrayDynamicIndexing: " << device_features.shaderStorageImageArrayDynamicIndexing << endl;
	cout << "shaderClipDistance: " << device_features.shaderClipDistance << endl;
	cout << "shaderCullDistance: " << device_features.shaderCullDistance << endl;
	cout << "shaderFloat64: " << device_features.shaderFloat64 << endl;
	cout << "shaderInt64: " << device_features.shaderInt64 << endl;
	cout << "shaderInt16: " << device_features.shaderInt16 << endl;
	cout << "shaderResourceResidency: " << device_features.shaderResourceResidency << endl;
	cout << "shaderResourceMinLod: " << device_features.shaderResourceMinLod << endl;
	cout << "sparseBinding: " << device_features.sparseBinding << endl;
	cout << "sparseResidencyBuffer: " << device_features.sparseResidencyBuffer << endl;
	cout << "sparseResidencyImage2D: " << device_features.sparseResidencyImage2D << endl;
	cout << "sparseResidencyImage3D: " << device_features.sparseResidencyImage3D << endl;
	cout << "sparseResidency2Samples: " << device_features.sparseResidency2Samples << endl;
	cout << "sparseResidency4Samples: " << device_features.sparseResidency4Samples << endl;
	cout << "sparseResidency8Samples: " << device_features.sparseResidency8Samples << endl;
	cout << "sparseResidency16Samples: " << device_features.sparseResidency16Samples << endl;
	cout << "sparseResidencyAliased: " << device_features.sparseResidencyAliased << endl;
	cout << "variableMultisampleRate: " << device_features.variableMultisampleRate << endl;
	cout << "inheritedQueries: " << device_features.inheritedQueries << endl;
	cout << endl << "-------------------------------------------" << endl << endl;
}

/*
for (size_t i = 0; i < div_cou; i++)
{
cout << endl;
cout << "==================================" << endl;
cout << endl;
VkPhysicalDevice cdevice = device_list[i];
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

printFeature(features);

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
*/

void printTime() {
	time_t tim;
	time(&tim);
	tm point;
	gmtime_s(&point, &tim);
	cout << "[" << point.tm_hour << ":" << point.tm_min << ":" << point.tm_sec << "]:";
}

void printVersion(int version) {
	cout << VK_VERSION_MAJOR(version) << "." << VK_VERSION_MINOR(version) << "." << VK_VERSION_PATCH(version);
}

void handel(VkResult result) {
	if (result != VK_SUCCESS) {
		cout << "FAILED WITH ERROR: " << result;
		throw new exception;
	}
}

void error(char* errorname, int errorcode) {
	cout << "Error occured (" << errorcode << ")" << endl << errorname << endl;
	_sleep(10000);
	exit(errorcode);
}