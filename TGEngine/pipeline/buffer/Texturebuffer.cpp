#include "Texturebuffer.hpp"

std::vector<Texture*> texture_buffers;
Descriptor texture_descriptor;
VkSampler tex_image_sampler;
uint32_t tex_array_index = 0;

using namespace nio;

void createTexture(Texture* tex) {
	TG_VECTOR_APPEND_NORMAL(texture_buffers, tex);
	tex->index = (uint32_t)last_size;
}

void initAllTextures() {
	VkSamplerCreateInfo sampler_create_info = {
		VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		nullptr,
		0,
		VK_FILTER_LINEAR,
		VK_FILTER_LINEAR,
		VK_SAMPLER_MIPMAP_MODE_LINEAR,
		VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		0,
		VK_TRUE,
		16.0f,
		VK_TRUE,
		VK_COMPARE_OP_ALWAYS,
		0,
		1,
		VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,
		VK_FALSE
	};
	last_result = vkCreateSampler(device, &sampler_create_info, nullptr, &tex_image_sampler);
	HANDEL(last_result)

	texture_descriptor = {
		VK_SHADER_STAGE_FRAGMENT_BIT,
		MAX_TEXTURES,
		VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		VK_NULL_HANDLE,
		tex_image_sampler,
		NULL
	};
	addDescriptor(&texture_descriptor);

	uint32_t buffer_index;
	FIND_INDEX(buffer_index, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)

	vlib_image_create_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	vlib_image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
	vlib_image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

	for each(Texture* ptr in texture_buffers) {

		if (ptr->texture_path) {
			File file = open(ptr->texture_path, "rb");
			ptr->image_data = stbi_load_from_file(file, &ptr->width, &ptr->height, &ptr->channel, STBI_rgb_alpha);
		}

		vlib_image_create_info.format = ptr->image_format;
		vlib_image_create_info.extent.width = ptr->width;
		vlib_image_create_info.extent.height = ptr->height;
		last_result = vkCreateImage(device, &vlib_image_create_info, nullptr, &ptr->image);
		HANDEL(last_result)

		vkGetImageMemoryRequirements(device, ptr->image, &ptr->requierments);

		vlib_buffer_memory_allocate_info.allocationSize = ptr->requierments.size;
		vlib_buffer_memory_allocate_info.memoryTypeIndex = vlib_device_local_memory_index;
		last_result = vkAllocateMemory(device, &vlib_buffer_memory_allocate_info, nullptr, &ptr->d_memory);
		HANDEL(last_result)

		last_result = vkBindImageMemory(device, ptr->image, ptr->d_memory, 0);
		HANDEL(last_result)

		vlib_buffer_create_info.size = ptr->width * ptr->height * 4;
		last_result = vkCreateBuffer(device, &vlib_buffer_create_info, nullptr, &ptr->buffer);
		HANDEL(last_result)

		vkGetBufferMemoryRequirements(device, ptr->buffer, &ptr->buffer_requierments);

		vlib_buffer_memory_allocate_info.allocationSize = ptr->buffer_requierments.size;
		vlib_buffer_memory_allocate_info.memoryTypeIndex = buffer_index;
		last_result = vkAllocateMemory(device, &vlib_buffer_memory_allocate_info, nullptr, &ptr->buffer_memory);
		HANDEL(last_result)

		last_result = vkBindBufferMemory(device, ptr->buffer, ptr->buffer_memory, 0);
		HANDEL(last_result)

		last_result = vkMapMemory(device, ptr->buffer_memory, 0, ptr->width * ptr->height * 4, 0, &ptr->memory);
		HANDEL(last_result);

		memcpy(ptr->memory, ptr->image_data, ptr->width * ptr->height * 4);
		vkUnmapMemory(device, ptr->buffer_memory);

		if (ptr->texture_path) {
			stbi_image_free(ptr->image_data);
		}
		else {
			delete[] ptr->image_data;
		}

		vlib_image_view_create_info.format = ptr->image_format;
		vlib_image_view_create_info.image = ptr->image;
		last_result = vkCreateImageView(device, &vlib_image_view_create_info, nullptr, &ptr->image_view);
		HANDEL(last_result)
	}
}

void addTextures() {
	uint32_t index = 0;
	texture_descriptor.image_view = new VkImageView[MAX_TEXTURES];
	for each (Texture* tex in texture_buffers) {
		texture_descriptor.image_view[index] = tex->image_view;
		index++;
	}
	for (; index < MAX_TEXTURES; index++) {
		texture_descriptor.image_view[index] = texture_buffers[0]->image_view;
	}
	texture_descriptor.count = MAX_TEXTURES;
	texture_descriptor.binding = 0;
	texture_descriptor.descriptor_set = 1;
	updateDescriptorSet(&texture_descriptor, 0);
	texture_descriptor.descriptor_set = 0;
	updateDescriptorSet(&texture_descriptor, 0);
}

void destroyBufferofTexture(Texture* tex) {
	vkFreeMemory(device, tex->buffer_memory, nullptr);
	vkDestroyBuffer(device, tex->buffer, nullptr);
}


void destroyTexture(Texture* tex) {
	vkDestroyImageView(device, tex->image_view, nullptr);
	vkFreeMemory(device, tex->d_memory, nullptr);
	vkDestroyImage(device, tex->image, nullptr);
}

void destroyAllTextures() {
	last_result = vkDeviceWaitIdle(device);
	HANDEL(last_result)

		vkDestroySampler(device, tex_image_sampler, nullptr);
	for each(Texture* tex in texture_buffers) {
		destroyTexture(tex);
	}
}