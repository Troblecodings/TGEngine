#pragma once

#include "Memory.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "../../../stb/stb_image.h"
#include "../Pipe.hpp"
#include "VertexBuffer.hpp"
#include "../../vlib/VulkanImage.hpp"
#include "../../vlib/VulkanBuffer.hpp"
#include "../../util/VectorUtil.hpp"
#include "../../game_content/Light.hpp"

#define AUTO_MIPMAP 0xFFFFFF

/*
 * Holder for Vulkan specific image components
 */
SINCE(0, 0, 4)
class VulkanTexture {

private:
	VkImage image; // The image vulkan representation
	VkImageView imageView; // The view of the image ... to view the image LOL
	VkBuffer buffer; // Buffer to store the image in befor it is transfered to the actual image -> deleted after transfer

	VkDeviceMemory imageMemory; // The bound image memory
	VkDeviceMemory bufferMemory; // The bound buffer memory -> deleted after transfer

	VkFormat imageFormat = VK_FORMAT_R8G8B8A8_UNORM; // TODO Needs auto querry

public:
	void initVulkan();

	void* map(uint32_t size);
	void unmap();
	void updateDescriptor();

	void dispose(); // Disposes unneeded resources
	void destroy(); // Destroys the whole texture
};

class Texture {

public:
	VulkanTexture vulkanTexture;

private:
	char* textureName;
	uint32_t miplevels = AUTO_MIPMAP;

	int width;
	int height;
	int channel;
	uint8_t* imageData;

public:
	Texture();

	void initTexture();
};

extern std::vector<Texture*> textures;
extern Descriptor textureDescriptor;
extern VkSampler imageSampler;

SINCE(0, 0, 2)
void initAllTextures();

SINCE(0, 0, 2)
void destroyAllTextures();