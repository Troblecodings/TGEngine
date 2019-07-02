#include "PrePipeline.hpp"
#include "window/Window.hpp"

std::vector<VkSurfaceFormatKHR> surface_format;
std::vector<VkPresentModeKHR> present_mode;
VkSurfaceFormatKHR used_format;
VkFormat used_depth_format = VK_FORMAT_UNDEFINED;
VkPresentModeKHR used_present_mode;
VkSampleCountFlagBits  used_msaa_flag;

void prePipeline() {
	uint32_t count = 0;

	lastResult = vkGetPhysicalDeviceSurfaceFormatsKHR(used_physical_device, window_list[0]->surface, &count, nullptr);
	HANDEL(lastResult)

		surface_format.resize(count);
	lastResult = vkGetPhysicalDeviceSurfaceFormatsKHR(used_physical_device, window_list[0]->surface, &count, surface_format.data());
	HANDEL(lastResult)

		lastResult = vkGetPhysicalDeviceSurfacePresentModesKHR(used_physical_device, window_list[0]->surface, &count, nullptr);
	HANDEL(lastResult)

		present_mode.resize(count);
	lastResult = vkGetPhysicalDeviceSurfacePresentModesKHR(used_physical_device, window_list[0]->surface, &count, present_mode.data());
	HANDEL(lastResult)

		used_msaa_flag = (VkSampleCountFlagBits)properties->getInt("msaa");

	VkSampleCountFlags counts = std::min(device_properties.limits.framebufferColorSampleCounts, device_properties.limits.framebufferDepthSampleCounts);
	if(!(counts & used_msaa_flag)) {
		if(counts & VK_SAMPLE_COUNT_2_BIT) { used_msaa_flag = VK_SAMPLE_COUNT_2_BIT; }
		if(counts & VK_SAMPLE_COUNT_4_BIT) { used_msaa_flag = VK_SAMPLE_COUNT_4_BIT; }
		if(counts & VK_SAMPLE_COUNT_8_BIT) { used_msaa_flag = VK_SAMPLE_COUNT_8_BIT; }
		if(counts & VK_SAMPLE_COUNT_16_BIT) { used_msaa_flag = VK_SAMPLE_COUNT_16_BIT; }
		if(counts & VK_SAMPLE_COUNT_32_BIT) { used_msaa_flag = VK_SAMPLE_COUNT_32_BIT; }
		if(counts & VK_SAMPLE_COUNT_64_BIT) { used_msaa_flag = VK_SAMPLE_COUNT_64_BIT; }
	}

	used_format = surface_format[0];
	for(VkSurfaceFormatKHR c_surface_formats : surface_format) {
		if(c_surface_formats.format == VK_FORMAT_B8G8R8A8_UNORM) {
			used_format = c_surface_formats;
			break;
		}
	}

	for(VkFormat format : { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}) {
		VkFormatProperties prop;
		vkGetPhysicalDeviceFormatProperties(used_physical_device, format, &prop);
		if((prop.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) == VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
			used_depth_format = format;
			break;
		}
	}

	if(used_depth_format == VK_FORMAT_UNDEFINED) {
		TGERROR(TG_ERR_DEPTH_FORMAT_NOT_SUPPORTED)
	}

	used_present_mode = present_mode[0];
	for(VkPresentModeKHR c_present_mode : present_mode) {
		if(c_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
			used_present_mode = c_present_mode;
			break;
		}
	}
}