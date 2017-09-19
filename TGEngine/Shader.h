#pragma once

#include "stdbase.h"
#include "Debug.h"
#include "Files.h"

namespace Pipeline {

	struct Shader {
		char* name;
		VkShaderModule* module;
		VkDevice* device;
		VkShaderStageFlagBits bits;
		VkAllocationCallbacks* allocator;
	};

	/*
	 * Creates a Shader 
	 */
	SINCE(0, 0, 6)
	void createShader(Shader* sh);

	/*
	* Destroys the Shader
	*/
	SINCE(0, 0, 6)
	void destroyShader(Shader* sh);
}