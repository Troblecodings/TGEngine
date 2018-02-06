#pragma once

#include "Memory.hpp"

struct Texture {
	char* texture_path;
	VkExtent2D texture_extend;
	VkImage image;

	uint32_t depth;
	uint32_t color_type;
	uint32_t compression_methode;
	uint32_t filter_methode;
	uint32_t interlace_methode;


	uint32_t ptr;
	void* memory;
};

SINCE(0, 0, 2)
void createTexture(Texture tex);