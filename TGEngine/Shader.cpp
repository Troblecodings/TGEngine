#include "Shader.h"

namespace Pipeline {

	using namespace std;

	void createShader(Shader* sh) {
		vector<char> buffer = nio::getBinarys(sh->name);

		VkShaderModuleCreateInfo shader_creatinfo = {};
		shader_creatinfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shader_creatinfo.codeSize = buffer.size();
		shader_creatinfo.pCode = (uint32_t*)buffer.data();

		sh->module = new VkShaderModule;
		handel(vkCreateShaderModule(*(sh->device), &shader_creatinfo, nullptr, sh->module));

		VkPipelineShaderStageCreateInfo shader_stage_create_info = {};
		shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shader_stage_create_info.module = *sh->module;
		shader_stage_create_info.stage = sh->bits;
		shader_stage_create_info.pName = "main";
		shader_stage_create_info.pSpecializationInfo = nullptr;
		sh->createInfo = shader_stage_create_info;
	}

	void destroyShader(Shader* sh) {
		vkDestroyShaderModule(*sh->device, *sh->module, nullptr);
	}

}