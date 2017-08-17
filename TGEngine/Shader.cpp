#include "Shader.h"

namespace Pipeline {

	using namespace std;

	void createShader(Shader* sh) {
		ifstream inputstream(sh->name, ios::binary | ios::ate);
		size_t size_of_file = inputstream.tellg();
		vector<char> binary_file;
		binary_file.resize(size_of_file);
		inputstream.seekg(0);
		inputstream.read(binary_file.data(), size_of_file);
		inputstream.close();

		VkShaderModuleCreateInfo shader_creatinfo = {};
		shader_creatinfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shader_creatinfo.codeSize = size_of_file;
		shader_creatinfo.pCode = (uint32_t*)binary_file.data();

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