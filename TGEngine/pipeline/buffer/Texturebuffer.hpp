#pragma once

#include "../../io/Files.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "vulkan/vulkan.h"
#include "../Device.hpp"

namespace tge::tex {

	struct TextureOutput{
		uint32_t id;
		int x;
		int y;
		int comp;
		VkImage image;
		VkImageView view;
		uint32_t samplerid
	};

	struct TextureIn {
		const unsigned long long size;
	};

	void createTextures(TextureIn* input, size_t size, TextureOutput* output);

}