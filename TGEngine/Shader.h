#pragma once

#include "stdbase.h"
#include "Debug.h"
#include "TPointer.h"
#include <fstream>

namespace Pipeline {

	struct Shader {
		char* name;
		VkShaderModule* module;
		VkDevice* device;
		VkPipelineShaderStageCreateInfo createInfo;
		VkShaderStageFlagBits bits;
		VkAllocationCallbacks* allocator;
	};

	/*
	 * Creates a Shader 
	 */
	void createShader(Shader* sh);

	/*
	* Destroys the Shader
	*/
	void destroyShader(Shader* sh);
}