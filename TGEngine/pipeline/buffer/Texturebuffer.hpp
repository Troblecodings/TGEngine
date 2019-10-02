#pragma once

#include "../../io/Files.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "vulkan/vulkan.h"
#include "../Device.hpp"

namespace tge::tex {

	struct TextureOutput{
		uint32_t id;
		VkImage image;
		VkImageView view;
	};

	struct TextureLoaded {
		stbi_uc* data;
		VkSampler sampler;
		int x;
		int y;
		int comp;
	};

	struct TextureIn {
		uint64_t offset;
		uint64_t size;
	};

	void loadTextures(TextureIn input, uint32_t size, TextureLoaded* loaded);

	void createTextures(TextureLoaded* input, uint32_t size, TextureOutput* output);

}