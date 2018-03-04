#include "PrePipeline.hpp"

std::vector<VkSurfaceFormatKHR> surface_format;
std::vector<VkPresentModeKHR> present_mode;
VkSurfaceFormatKHR used_format;
VkPresentModeKHR used_present_mode;

void prePipeline() {
	uint32_t count = 0;

	last_result = vkGetPhysicalDeviceSurfaceFormatsKHR(used_physical_device, surface, &count, nullptr);
	HANDEL(last_result)

	surface_format.resize(count);
	last_result = vkGetPhysicalDeviceSurfaceFormatsKHR(used_physical_device, surface, &count, surface_format.data());
	HANDEL(last_result)

	last_result = vkGetPhysicalDeviceSurfacePresentModesKHR(used_physical_device, surface, &count, nullptr);
	HANDEL(last_result)

	present_mode.resize(count);
	last_result = vkGetPhysicalDeviceSurfacePresentModesKHR(used_physical_device, surface, &count, present_mode.data());
	HANDEL(last_result)

	used_format = surface_format[0];
	for (VkSurfaceFormatKHR c_surface_formats : surface_format) {
		if (c_surface_formats.format == VK_FORMAT_B8G8R8A8_UNORM) {
			used_format = c_surface_formats;
		}
	}

	used_present_mode = present_mode[0];
	for (VkPresentModeKHR c_present_mode : present_mode) {
		if (c_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
			used_present_mode = c_present_mode;
		}
	}
}