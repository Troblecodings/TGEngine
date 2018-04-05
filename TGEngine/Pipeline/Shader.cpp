#include "Shader.hpp"

std::vector<VkPipelineShaderStageCreateInfo> shaders;
std::vector<VkVertexInputAttributeDescription> description_attributes;

void createShader() {
	uint32_t pos = 0;
	for each(std::vector<char> data in shader_data) {
		VkShaderModule shader_module = {};
		VkShaderModuleCreateInfo info = {
			VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			nullptr,
			0,
			data.size(),
			reinterpret_cast<const uint32_t*>(data.data())
		};
		last_result = vkCreateShaderModule(device, &info, nullptr, &shader_module);
		HANDEL(last_result)
		size_t size = shaders.size();
		shaders.resize(size + 1);
		shaders[size] = {
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			nullptr,
			0,
			shader_flags[pos],
			shader_module,
			"main",
			nullptr
		};
		pos++;
	}
}

void createShaderInput(uint32_t location, uint32_t offset, VkFormat format) {
	size_t size = description_attributes.size();
	description_attributes.resize(size + 1);
	description_attributes[size] = {
		location,
		0,
		format,
		offset
	};
}

void destroyShaders() {
	for (VkPipelineShaderStageCreateInfo stage_info : shaders) {
		vkDestroyShaderModule(device, stage_info.module, nullptr);
	}
}