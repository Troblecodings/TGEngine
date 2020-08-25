#include "../../public/pipeline/Instance.hpp"

VkResult lastResult;
VkInstance instance;
VkDebugUtilsMessengerEXT debugMessager;

void createInstance() {

	VkApplicationInfo applicationInfo;
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pNext = nullptr;
	applicationInfo.pApplicationName = tgeproperties.getStringOrDefault("app_name", "TGEngine");
	applicationInfo.applicationVersion = (uint32_t)tgeproperties.getInt("version");
	applicationInfo.pEngineName = "TGEngine";
	applicationInfo.engineVersion = TGE_VERSION;
	applicationInfo.apiVersion = VK_API_VERSION_1_0;

	// Layer list
	constexpr char* layersToEnable[] = {
	#ifdef DEBUG 
		"VK_LAYER_LUNARG_standard_validation",
		"VK_LAYER_LUNARG_assistant_layer",
		"VK_LAYER_LUNARG_monitor",
	#endif
		"VK_LAYER_VALVE_steam_overlay",
		"VK_LAYER_NV_optimus",
		"VK_LAYER_AMD_switchable_graphics"
	};

	//Validation for the instance layers
	uint32_t layerCount = 0;
	std::vector<const char*> enabledLayerNames;
	CHECKFAIL(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));
	enabledLayerNames.reserve(layerCount);
	VkLayerProperties* layerProperties = new VkLayerProperties[layerCount];
	CHECKFAIL(vkEnumerateInstanceLayerProperties(&layerCount, layerProperties));
	for (uint32_t i = 0; i < layerCount; i++) {
		VkLayerProperties layer = layerProperties[i];
		OUT_LV_DEBUG("Available " << layer.layerName);
		for (const char* name : layersToEnable) {
			if (strcmp(layer.layerName, name) == 0) {
				enabledLayerNames.push_back(name);
				OUT_LV_DEBUG("Activate Layer: " << name);
				break;
			}
		}
	}

	// Extension list
	const char* extensionsToEnable[] = {
#ifdef DEBUG
	VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif // DEBUG
#ifdef _WIN32
	VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif // _WIN
	VK_KHR_SURFACE_EXTENSION_NAME
	};

	//Validation for the intance extensions
	uint32_t extensionCount = 0;
	std::vector<const char*> enabledExtensionNames;
	CHECKFAIL(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr));
	enabledExtensionNames.reserve(extensionCount);
	VkExtensionProperties* usableExtensionNames = new VkExtensionProperties[extensionCount];
	CHECKFAIL(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, usableExtensionNames));
	for (uint32_t i = 0; i < extensionCount; i++) {
		VkExtensionProperties extension = usableExtensionNames[i];
		OUT_LV_DEBUG("Available " << extension.extensionName);
		for (const char* name : extensionsToEnable) {
			if (strcmp(extension.extensionName, name) == 0) {
				enabledExtensionNames.push_back(name);
				OUT_LV_DEBUG("Active " << name);
				break;
			}
		}

	}

	//Create Instance
	VkInstanceCreateInfo instanceCreateInfo;
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext = nullptr;
	instanceCreateInfo.flags = 0;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
	instanceCreateInfo.enabledLayerCount = (uint32_t)enabledLayerNames.size();
	instanceCreateInfo.ppEnabledLayerNames = enabledLayerNames.data();
	instanceCreateInfo.enabledExtensionCount = (uint32_t)enabledExtensionNames.size();
	instanceCreateInfo.ppEnabledExtensionNames = enabledExtensionNames.data();
	CHECKFAIL(vkCreateInstance(&instanceCreateInfo, nullptr, &instance));

#ifdef DEBUG
	/*
	 * Create a debug messenger
	 */
	VkDebugUtilsMessengerCreateInfoEXT utilMessagerCreateInfo;
	utilMessagerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	utilMessagerCreateInfo.flags = 0;
	utilMessagerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	utilMessagerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
	utilMessagerCreateInfo.pfnUserCallback = debugCallback;
	utilMessagerCreateInfo.pUserData = 0;
	utilMessagerCreateInfo.pNext = 0;

	PFN_vkCreateDebugUtilsMessengerEXT createDebugReportCallback = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	OUT_LV_DEBUG(createDebugReportCallback);
	if (!createDebugReportCallback) {
		TGERROR(TG_ERR_DB_DBMESSAGER_NOT_VALID)
	}
	CHECKFAIL(createDebugReportCallback(instance, &utilMessagerCreateInfo, nullptr, &debugMessager));
#endif
}

#ifdef DEBUG
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT                  messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	switch (messageSeverity) {
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		std::cout << "VERBOS [";
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		std::cout << "INFO [";
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		std::cout << "WARNING [";
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		std::cout << "ERROR [";
		break;
	default:
		break;
	}

	switch (messageType) {
	case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
		std::cout << "GENERAL]: ";
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
		std::cout << "PERFORMANCE]: ";
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
		std::cout << "VALIDATION]: ";
		break;
	default:
		break;
	}

	std::cout << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}
#endif // DEBUG

void destroyInstance() {
#ifdef DEBUG
	PFN_vkDestroyDebugUtilsMessengerEXT destroyDebugReportCallback = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (!destroyDebugReportCallback) {
		TGERROR(TG_ERR_DB_DBMESSAGER_NOT_VALID)
	}
	destroyDebugReportCallback(instance, debugMessager, nullptr);
#endif
	vkDestroyInstance(instance, nullptr);
}