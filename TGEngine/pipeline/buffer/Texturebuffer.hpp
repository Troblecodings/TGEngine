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
	};

	struct TextureIn {
		uint64_t offset;
		uint64_t size;
		VkSampler sampler;
	};

	void createTextures(TextureIn* input, uint32_t size, TextureOutput* output);

}