#include "Instance.hpp"

VkResult last_result;
VkInstance instance;
VkDebugUtilsMessengerEXT msger;

void createInstance(std::vector<const char*> layers_to_enable, std::vector<const char*> extensions_to_enable) {

	VkApplicationInfo app_info = { 
		VK_STRUCTURE_TYPE_APPLICATION_INFO,
		nullptr,
		properties->getStringOrDefault("app_name", "TGEngine"),
		(uint32_t)properties->getInt("version"),
	    "TGEngine",
	    TGE_VERSION,
		VK_API_VERSION_1_1
	};

	uint32_t count;
	//Validation for the instance layers
	std::vector<const char*> enable_layer;
	if (layers_to_enable.size() > 0) {
		last_result = vkEnumerateInstanceLayerProperties(&count, nullptr);
		HANDEL(last_result)
		std::vector<VkLayerProperties> usable_layers(count);
		last_result = vkEnumerateInstanceLayerProperties(&count, usable_layers.data());
		HANDEL(last_result)
		for each(VkLayerProperties layer in usable_layers) {
			OUT_LV_DEBUG("Available " << layer.layerName)
			for each(const char* name in layers_to_enable) {
				if (strcmp(layer.layerName, name) == 0) {
					TG_VECTOR_APPEND_NORMAL(enable_layer, name)
					OUT_LV_DEBUG("Active " << name)
					break;
				}
			}
		}
		layers_to_enable.clear();
		usable_layers.clear();
	}

    #ifdef _WIN32
		TG_VECTOR_APPEND_NORMAL(extensions_to_enable, VK_KHR_SURFACE_EXTENSION_NAME)
		TG_VECTOR_APPEND_NORMAL(extensions_to_enable, VK_KHR_WIN32_SURFACE_EXTENSION_NAME)
    #endif

	//Validation for the intance extensions
	std::vector<const char*> enable_extensions;
	if (extensions_to_enable.size() > 0) {
		last_result = vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
		HANDEL(last_result)
		std::vector<VkExtensionProperties> usable_extensions(count);
		last_result = vkEnumerateInstanceExtensionProperties(nullptr, &count, usable_extensions.data());
		HANDEL(last_result)
		for each(VkExtensionProperties extension in usable_extensions) {	
			OUT_LV_DEBUG("Available " << extension.extensionName)
			for each(const char* name in extensions_to_enable) {
				if (strcmp(extension.extensionName, name) == 0) {
					TG_VECTOR_APPEND_NORMAL(enable_extensions, name)
					OUT_LV_DEBUG("Active " << name)
					break;
				}
			}
		}
		extensions_to_enable.clear();
		usable_extensions.clear();
	}
	
	//Create Instance
	VkInstanceCreateInfo instance_create_info = {
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
	    nullptr,
	    0,
	    &app_info,
		(uint32_t)enable_layer.size(),
	    enable_layer.data(),
		(uint32_t)enable_extensions.size(),
	    enable_extensions.data()
	};
	last_result = vkCreateInstance(&instance_create_info, nullptr, &instance);
	HANDEL(last_result)

#ifdef DEBUG
	VkDebugUtilsMessengerCreateInfoEXT util_message = {
		VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		nullptr,
		0,
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
		&callback_debug,
		VK_NULL_HANDLE
	};
	PFN_vkCreateDebugUtilsMessengerEXT CreateDebugReportCallback = VK_NULL_HANDLE;
	CreateDebugReportCallback = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	last_result = CreateDebugReportCallback(instance, &util_message, nullptr, &msger);
	HANDEL(last_result)
#endif
}

#ifdef DEBUG
VkBool32 callback_debug(
	VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT                  messageType,
	const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
	void *pUserData) {
	OUT_LV_DEBUG(pCallbackData->pMessage)
		return true;
}
#endif // DEBUG

void destroyInstance() {
	vkDestroyInstance(instance, nullptr);
}