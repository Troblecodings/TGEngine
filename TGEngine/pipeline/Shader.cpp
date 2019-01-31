#include "Shader.hpp"

std::vector<VkPipelineShaderStageCreateInfo> shaders;
std::vector<VkVertexInputAttributeDescription> description_attributes;

void createShader() {
	for each(std::vector<char> data in shader_data) {
		VkShaderModule shader_module;
		VkShaderModuleCreateInfo info = {
			VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			nullptr,
			0,
			data.size(),
			reinterpret_cast<const uint32_t*>(data.data())
		};
		last_result = vkCreateShaderModule(device, &info, nullptr, &shader_module);
		HANDEL(last_result)
		TG_VECTOR_APPEND_NORMAL(shaders, VkPipelineShaderStageCreateInfo())
		shaders[last_size] = {
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			nullptr,
			0,
			shader_flags[last_size],
			shader_module,
			"main",
			nullptr
		};
	}
}

void createShaderInput(uint32_t location, uint32_t offset, VkFormat format) {
	TG_VECTOR_APPEND_NORMAL(description_attributes, VkVertexInputAttributeDescription())
	description_attributes[last_size].location = location;
	description_attributes[last_size].offset = offset;
	description_attributes[last_size].format = format;
	description_attributes[last_size].binding = 0;
}

void destroyShaders() {
	for (VkPipelineShaderStageCreateInfo stage_info : shaders) {
		vkDestroyShaderModule(device, stage_info.module, nullptr);
	}
}