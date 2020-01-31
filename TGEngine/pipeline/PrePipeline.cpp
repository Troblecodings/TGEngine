#include "PrePipeline.hpp"
#include "window/Window.hpp"

VkSurfaceFormatKHR usedSurfaceFormat;
VkFormat usedDepthFormat = VK_FORMAT_UNDEFINED;
VkPresentModeKHR usedPresentMode;
VkSampleCountFlagBits  usedSampleFlag;

void prePipeline() {
	uint32_t surfaceFormatCount = 0;
	CHECKFAIL(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, tge::win::windowSurface, &surfaceFormatCount, nullptr));

	VkSurfaceFormatKHR* surfaceFormatKHRs = new VkSurfaceFormatKHR[surfaceFormatCount];
	CHECKFAIL(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, tge::win::windowSurface, &surfaceFormatCount, surfaceFormatKHRs));

	uint32_t surfacePresentModeCount = 0;
	CHECKFAIL(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, tge::win::windowSurface, &surfacePresentModeCount, nullptr));

	VkPresentModeKHR* presentModeKHRs = new VkPresentModeKHR[surfacePresentModeCount];
	CHECKFAIL(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, tge::win::windowSurface, &surfacePresentModeCount, presentModeKHRs));

	usedSampleFlag = (VkSampleCountFlagBits)tgeproperties.getInt("msaa");

	VkSampleCountFlags sampleCountFlag = TGE_MIN(deviceProperties.limits.framebufferColorSampleCounts, deviceProperties.limits.framebufferDepthSampleCounts);
	if (!(sampleCountFlag & usedSampleFlag)) {
		if (sampleCountFlag & VK_SAMPLE_COUNT_64_BIT) usedSampleFlag = VK_SAMPLE_COUNT_64_BIT;
		else if (sampleCountFlag & VK_SAMPLE_COUNT_32_BIT) usedSampleFlag = VK_SAMPLE_COUNT_32_BIT;
		else if (sampleCountFlag & VK_SAMPLE_COUNT_16_BIT) usedSampleFlag = VK_SAMPLE_COUNT_16_BIT;
		else if (sampleCountFlag & VK_SAMPLE_COUNT_8_BIT) usedSampleFlag = VK_SAMPLE_COUNT_8_BIT;
		else if (sampleCountFlag & VK_SAMPLE_COUNT_4_BIT) usedSampleFlag = VK_SAMPLE_COUNT_4_BIT;
		else if (sampleCountFlag & VK_SAMPLE_COUNT_2_BIT) usedSampleFlag = VK_SAMPLE_COUNT_2_BIT;
	}

	usedSurfaceFormat = surfaceFormatKHRs[0];
	for (uint32_t i = 0; i < surfaceFormatCount; i++) {
		if (surfaceFormatKHRs[i].format == VK_FORMAT_B8G8R8A8_UNORM) {
			usedSurfaceFormat = surfaceFormatKHRs[i];
			break;
		}
	}

	for (VkFormat format : { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}) {
		VkFormatProperties prop;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &prop);
		if ((prop.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) == VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
			usedDepthFormat = format;
			break;
		}
	}

	if (usedDepthFormat == VK_FORMAT_UNDEFINED) {
		TGERROR(TG_ERR_DEPTH_FORMAT_NOT_SUPPORTED)
	}

	usedPresentMode = presentModeKHRs[0];
	for (uint32_t i = 0; i < surfacePresentModeCount; i++) {
		if (presentModeKHRs[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
			usedPresentMode = presentModeKHRs[i];
			break;
		}
	}
}