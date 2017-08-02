#pragma once

#include "stdbase.h"
#include "Debug.h"
#include "TPointer.h"
#include "TStorage.h"

namespace Pipeline {

	struct Shader {
		char* name;
		VkShaderModule* module;
		VkDevice* device;
		IStorage* files;
	};

}