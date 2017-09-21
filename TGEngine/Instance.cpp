#include "Instance.hpp"

VkResult last_result;
VkInstance instance;
VkAllocationCallbacks allocator;

void createInstance(nio::Properties propertys, std::vector<char*> layers_to_enable, std::vector<char*> extensions_to_enable) {

	VkApplicationInfo app_info = { 
		VK_STRUCTURE_TYPE_APPLICATION_INFO, 
		nullptr,
		propertys.getString("app_name").value,
		propertys.getInt("version").rvalue,
	    "TGEngine",
	    VK_MAKE_VERSION(0,0,1),
	    VK_VERSION_1_0
	};

	uint32_t count;
	//Validation for the instance layers
	std::vector<char*> enable_layer;
	if (layers_to_enable.size() > 0) {
		last_result = vkEnumerateInstanceLayerProperties(&count, nullptr);
		HANDEL(last_result)
		std::vector<VkLayerProperties> usable_layers(count);
		last_result = vkEnumerateInstanceLayerProperties(&count, usable_layers.data());
		HANDEL(last_result)
		for each(VkLayerProperties layer in usable_layers) {
			for each(char* name in layers_to_enable) {
				if (layer.layerName == name) {
					uint32_t size = enable_layer.size();
					enable_layer.resize(size + 1);
					enable_layer[size] = name;
					break;
				}
			}
		}
	}

	//Validation for the intance extensions
	std::vector<char*> enable_extensions;
	if (extensions_to_enable.size() > 0) {
		last_result = vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
		HANDEL(last_result)
		std::vector<VkExtensionProperties> usable_extensions(count);
		last_result = vkEnumerateInstanceExtensionProperties(nullptr, &count, usable_extensions.data());
		HANDEL(last_result)
		for each(VkExtensionProperties extension in usable_extensions) {
			for each(char* name in extensions_to_enable) {
				if (extension.extensionName == name) {
					uint32_t size = enable_extensions.size();
					enable_extensions.resize(size + 1);
					enable_extensions[size] = name;
					break;
				}
			}
		}
	}
	
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