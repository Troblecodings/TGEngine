#include "Instance.hpp"

VkResult last_result;
VkInstance instance;

void createInstance(std::vector<const char*> layers_to_enable, std::vector<const char*> extensions_to_enable) {

	VkApplicationInfo app_info = { 
		VK_STRUCTURE_TYPE_APPLICATION_INFO,
		nullptr,
		properties->getString("app_name"),
		properties->getInt("version"),
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
			for each(const char* name in layers_to_enable) {
				if (std::string(layer.layerName).compare(name) == 0) {
					uint32_t size = enable_layer.size();
					enable_layer.resize(size + 1);
					enable_layer[size] = name;
					OUT_LV_DEBUG(name)
					break;
				}
			}
		}
		layers_to_enable.clear();
		usable_layers.clear();
	}

    #ifdef _WIN32
		count = extensions_to_enable.size();
	    extensions_to_enable.resize(count + 2);
		extensions_to_enable[count] = "VK_KHR_surface";
		extensions_to_enable[count + 1] = "VK_KHR_win32_surface";
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
			for each(const char* name in extensions_to_enable) {
				if (std::string(extension.extensionName).compare(name) == 0) {
					uint32_t size = enable_extensions.size();
					enable_extensions.resize(size + 1);
					enable_extensions[size] = name;
					OUT_LV_DEBUG(name)
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
	    enable_layer.size(),
	    enable_layer.data(),
	    enable_extensions.size(),
	    enable_extensions.data()
	};
	last_result = vkCreateInstance(&instance_create_info, nullptr, &instance);
	HANDEL(last_result)
}

void destroyInstance() {
	vkDestroyInstance(instance, nullptr);
}