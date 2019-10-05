#pragma once

#include "../../io/Files.hpp"
#include "vulkan/vulkan.h"
#include "../Device.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "../../../stb/stb_image.h"

namespace tge::tex {

	using namespace tge::nio;

	struct ResourceDescriptor {
		uint64_t offset;
		uint64_t size;
	};

	struct SamplerLoaded {

	};

	struct TextureOutput{
		uint32_t id;
		VkImage image;
		VkImageView view;
		VkDeviceMemory imagememory;
	};

	struct TextureLoaded {
		uint8_t* data;
		VkSampler sampler;
		int x;
		int y;
		int comp;
	};

	void loadSampler(File file, ResourceDescriptor* input, uint32_t size, SamplerLoaded* loaded);

	void createSampler(SamplerLoaded* input, uint32_t size, VkSampler* sampler);

	void loadTextures(File file, ResourceDescriptor* input, uint32_t size, TextureLoaded* loaded);

	void createTextures(TextureLoaded* input, uint32_t size, TextureOutput* output);

}