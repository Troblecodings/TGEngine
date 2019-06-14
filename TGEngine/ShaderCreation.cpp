#include "ShaderCreation.hpp"

VkPipelineShaderStageCreateInfo createShader(const unsigned char data[], VkShaderStageFlagBits flag, uint32_t size) {
	VkShaderModule shader_module;
	VkShaderModuleCreateInfo info = {
		VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		nullptr,
		0,
		size,
		reinterpret_cast<const uint32_t*>(data)
	};
	lastResult = vkCreateShaderModule(device, &info, nullptr, &shader_module);
	HANDEL(lastResult)
	return {
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			nullptr,
			0,
			flag,
			shader_module,
			"main",
			nullptr
	};
}