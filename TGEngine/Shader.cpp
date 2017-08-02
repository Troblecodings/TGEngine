#include "Shader.h"

namespace Pipeline {

	void createShader(Shader sh) {

		char* buffer;
		TPointer* ptr;
		getPointer(sh.files, sh.name, ptr);
		treadFile(ptr, buffer);

		VkShaderModuleCreateInfo ver_creatinfo = {};
		ver_creatinfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		ver_creatinfo.codeSize = strlen(buffer);
		ver_creatinfo.pCode = (uint32_t*)buffer;

		handel(vkCreateShaderModule(*(sh.device), &ver_creatinfo, nullptr, sh.module));
		if (!sh.module) {
			error("Can't creat Shader Module", -5);
		}
	}

}