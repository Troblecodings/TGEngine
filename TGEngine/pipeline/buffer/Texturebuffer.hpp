#pragma once

#include "../../io/Files.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "vulkan/vulkan.h"
#include "../Device.hpp"

namespace tge::tex {

	struct TextureOutput{
		unsigned int id;
		int x;
		int y;
		int comp;
	};

	struct TextureIn {
		const unsigned long long size;
	};

	void createTextures(TextureIn* input, size_t size, TextureOutput* output);

}