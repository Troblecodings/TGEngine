#pragma once

#include "Memory.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "../../../stb/stb_image.h"
#include "../Descriptors.hpp"
#include "VertexBuffer.hpp"
#include "../Draw.hpp"
#include "../../vlib/VulkanImage.hpp"
#include "../../vlib/VulkanBuffer.hpp"
#include "../../util/VectorUtil.hpp"

#define MAX_TEXTURES 4094
#define COLOR_ONLY MAX_TEXTURES + 1

struct Texture {
	INPUT 
	char* texture_path;
	OPT VkFormat image_format = VK_FORMAT_R8G8B8A8_UNORM;

	OUTPUT 
	int width;
	int height;
	int channel;
	stbi_uc* image_data;
	VkImage image;
	VkImageView image_view;
	VkBuffer buffer;
	VkDeviceMemory d_memory;
	VkDeviceMemory buffer_memory;
	VkMemoryRequirements buffer_requierments;
	VkMemoryRequirements requierments;
	uint32_t index;
	void* memory;

	DEPRECATED
	VkExtent2D texture_extend;
	uint32_t depth;
	uint32_t color_type;
	uint32_t compression_methode;
	uint32_t filter_methode;
	uint32_t interlace_methode;
	END_DEPRECATED

};

extern std::vector<Texture*> texture_buffers;
extern Descriptor texture_descriptor;
extern VkSampler tex_image_sampler;
extern uint32_t tex_array_index;

SINCE(0, 0, 2)
void createTexture(Texture* tex);

SINCE(0, 0, 2)
void initAllTextures();

SINCE(0, 0, 3)
void addTextures();

SINCE(0, 0, 2)
void destroyBufferofTexture(Texture* tex);

SINCE(0, 0, 2)
void destroyTexture(Texture* tex);

SINCE(0, 0, 2)
void destroyAllTextures();