#include "Shader.h"

namespace Pipeline {

	using namespace std;

	void createShader(Shader* sh) {
		vector<char> buffer = nio::getBinarys(sh->name);

		VkShaderModuleCreateInfo shader_creatinfo = {};
		shader_creatinfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shader_creatinfo.codeSize = buffer.size();
		shader_creatinfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

		sh->module = new VkShaderModule;
		handel(vkCreateShaderModule(*(sh->device), &shader_creatinfo, nullptr, sh->module));
	}

	void destroyShader(Shader* sh) {
		vkDestroyShaderModule(*sh->device, *sh->module, nullptr);
	}

}