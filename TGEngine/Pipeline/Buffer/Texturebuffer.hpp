#pragma once

#include "Memory.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "..\..\..\stb\stb_image.h"
#include "..\Descriptors.hpp"

struct Texture {
	INPUT 
	char* texture_path;
	
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
	void* memory;

	DEPRECATED
	VkExtent2D texture_extend;
	uint32_t depth;
	uint32_t color_type;
	uint32_t compression_methode;
	uint32_t filter_methode;
	uint32_t interlace_methode;
	END_DEPRECATED

	SINCE(0, 0, 2)
	void addBarrier(VkCommandBuffer buffer, VkImageLayout oldLayout, VkImageLayout newLayout);

};

extern std::vector<Texture*> texture_buffers;
extern Descriptor* texture_descriptor;
extern VkSampler tex_image_sampler;

SINCE(0, 0, 2)
void createTexture(Texture* tex);

SINCE(0, 0, 2)
void initAllTextures();

SINCE(0, 0, 3)
void setTexture(Texture* tex);

SINCE(0, 0, 2)
void destroyBufferofTexture(Texture* tex);

SINCE(0, 0, 2)
void destroyTexture(Texture* tex);

SINCE(0, 0, 2)
void destroyAllTextures();