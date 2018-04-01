#include "Texturebuffer.hpp"

std::vector<Texture*> texture_buffers;

void createTexture(Texture* tex) {
	uint32_t size = texture_buffers.size();
	texture_buffers.resize(size + 1);
	texture_buffers[size] = tex;
}

void initAllTextures() {
	for each(Texture* ptr in texture_buffers) {
		ptr->image_data = stbi_load(ptr->texture_path, &ptr->width, &ptr->height, &ptr->channel, STBI_rgb_alpha);
		VkImageCreateInfo image_create_info = {
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			nullptr,
			0,
			VK_IMAGE_TYPE_2D,
			VK_FORMAT_R8G8B8A8_UNORM,
		{
			ptr->width,
			ptr->height,
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
		last_result = vkCreateImage(device, &image_create_info, nullptr, &ptr->image);
		HANDEL(last_result)

		uint32_t index;
		for (index = 0; index < memory_properties.memoryTypeCount; index++) {
			if (memory_properties.memoryTypes[index].propertyFlags & (VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) break;
		}

		vkGetImageMemoryRequirements(device, ptr->image, &ptr->requierments);
		VkMemoryAllocateInfo mem_alloc_info = {
			VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		    nullptr,
			ptr->requierments.size,
			index
		};
		last_result = vkAllocateMemory(device, &mem_alloc_info, nullptr, &ptr->d_memory);
		HANDEL(last_result)

		last_result = vkBindImageMemory(device, ptr->image, ptr->d_memory, 0);
		HANDEL(last_result)

		vkMapMemory(device, ptr->d_memory, 0, ptr->width * ptr->height * 4, 0, &ptr->memory);
		memcpy(ptr->memory, ptr->image_data, ptr->width * ptr->height * 4);
		vkUnmapMemory(device, ptr->d_memory);

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