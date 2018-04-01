#include "Texturebuffer.hpp"

std::vector<Texture*> texture_buffers;

void createTexture(Texture* tex) {

	tex->image_data = stbi_load(tex->texture_path, &tex->width, &tex->height, &tex->channel, STBI_rgb_alpha);
	VkImageCreateInfo image_create_info = {
		VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		nullptr,
		0,
		VK_IMAGE_TYPE_2D,
		VK_FORMAT_R8G8B8A8_UNORM,
		{
		   width,
		   height,
		   1
		},
		1,
		1,
		VK_SAMPLE_COUNT_1_BIT,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		0,
		nullptr,
		VK_IMAGE_LAYOUT_UNDEFINED
	};
	VkImage image;
	last_result = vkCreateImage(device, &image_create_info, nullptr, &image);
	HANDEL(last_result)
	tex->image_ptr = getImageMemoryRequirements(image, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	uint32_t size = texture_buffers.size();
	texture_buffers.resize(size + 1);
	texture_buffers[size] = tex;
}

void initAllTextures() {
	for each(Texture* ptr in texture_buffers) {
		mapMemory(ptr->image_ptr, &ptr->memory);
		memcpy(ptr->memory, ptr->image_data, ptr->width * ptr->height * 4);
		unmapMemory();
		stbi_image_free(ptr->image_data);
	}
}

void destroyTexture(Texture* tex) {
	
}

void destroyAllTextures() {
	for each(Texture* tex in texture_buffers) {
		destroyTexture(tex);
	}
}