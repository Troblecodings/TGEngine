#pragma once

#include "../../io/Files.hpp"
#include "vulkan/vulkan.h"
#include "../Device.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "../../../stb/stb_image.h"

namespace tge::tex {

	using namespace tge::nio;

	struct TextureDefaults {
		VkSampler sampler;
		float     anisotropyFilter;
		uint32_t  mipmapLevels;
	};

	extern TextureDefaults defaults;

	struct SamplerInputInfo {
		VkSamplerAddressMode uSamplerMode;
		VkSamplerAddressMode vSamplerMode;
		VkFilter             filterMagnification;
		VkFilter             filterMignification;
	};

	struct Texture {
		uint32_t       id;
		VkImage        image;
		VkImageView    view;
		VkDeviceMemory imagememory;
	};

	struct TextureInputInfo {
		uint8_t* data;
		int       x;
		int       y;
		int       comp;
	};

	void loadSampler(File file, ResourceDescriptor* input, uint32_t size, SamplerInputInfo* loaded);

	void createSampler(SamplerInputInfo input);

	void loadTextures(File file, ResourceDescriptor* input, uint32_t size, TextureInputInfo* loaded);

	void createTextures(TextureInputInfo* input, uint32_t size, Texture* output);

}