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

class Material {

public:

	Texture* texture;
	glm::vec4 color;

	bool isUI = false;

	uint32_t pipeline_index;
	uint32_t descriptor_index;
	uint32_t layout_index;

	virtual void createMaterial();
	virtual void createUIMaterial();

	void destroy();

	bool operator==(const Material& material);
};

struct RenderOffsets {

	uint32_t material; // index in the @materials array of the material to use
	
	uint32_t size; // count of vertices to draw for this matirial
	uint32_t offset; // the offset at wich this material starts (global)
};

extern std::vector<Texture*> texture_buffers;
extern std::vector<Material> materials;
extern std::vector<RenderOffsets> render_offset;
extern Descriptor texture_descriptor;
extern VkSampler tex_image_sampler;
extern uint32_t tex_array_index;

SINCE(0, 0, 2)
void initAllTextures();

SINCE(0, 0, 2)
void destroyBufferofTexture(Texture* tex);

SINCE(0, 0, 2)
void destroyAllTextures();